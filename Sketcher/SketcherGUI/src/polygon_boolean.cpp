#include "polygon_boolean.h"
#include <QDebug>

using namespace std;

using Vertex = PolygonBoolean::GHVertex;

vector<Vertex*> PolygonBoolean::buildLinkedList(const vector<QPointF>& polygon) {
    vector<Vertex*> list;
    for (const QPointF& pt : polygon) list.push_back(new Vertex(pt));
    int n = list.size();
    for (int i = 0; i < n; ++i) {
        list[i]->next = list[(i + 1) % n];
        list[i]->prev = list[(i + n - 1) % n];
    }
    return list;
}

bool PolygonBoolean::segmentsIntersect(const QPointF& a1, const QPointF& a2,
                                       const QPointF& b1, const QPointF& b2,
                                       QPointF& out)
{
    double dx1 = a2.x() - a1.x();
    double dy1 = a2.y() - a1.y();
    double dx2 = b2.x() - b1.x();
    double dy2 = b2.y() - b1.y();

    double denom = dx1 * dy2 - dy1 * dx2;
    if (abs(denom) < 1e-8) return false; // parallel

    double ua = ((b1.x() - a1.x()) * dy2 - (b1.y() - a1.y()) * dx2) / denom;
    double ub = ((b1.x() - a1.x()) * dy1 - (b1.y() - a1.y()) * dx1) / denom;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) return false;

    out = QPointF(a1.x() + ua * dx1, a1.y() + ua * dy1);
    return true;
}

bool PolygonBoolean::findIntersections(vector<Vertex*>& subj, vector<Vertex*>& clip) {
    bool found = false;
    qDebug() << "Entering findIntersections()";
    Vertex* aStart = subj[0];
    Vertex* a = aStart;

    do {
        if (a->isIntersection) {
            a = a->next;
            continue;
        }

        Vertex* bStart = clip[0];
        Vertex* b = bStart;

        do {
            if (b->isIntersection) {
                b = b->next;
                continue;
            }

            QPointF inter;
            if (segmentsIntersect(a->point, a->next->point, b->point, b->next->point, inter)) {
                Vertex* i1 = new Vertex(inter);
                Vertex* i2 = new Vertex(inter);
                i1->isIntersection = i2->isIntersection = true;
                i1->neighbor = i2;
                i2->neighbor = i1;

                // insert into subject polygon
                Vertex* oldNext = a->next;
                a->next = i1;
                i1->prev = a;
                i1->next = oldNext;
                oldNext->prev = i1;

                // insert into clip polygon
                Vertex* oldNextB = b->next;
                b->next = i2;
                i2->prev = b;
                i2->next = oldNextB;
                oldNextB->prev = i2;

                qDebug() << "Inserted i1:" << i1->point << " prev:" << i1->prev->point << " next:" << i1->next->point;
                
                found = true;
            }

            b = b->next;
        } while (b != bStart);

        a = a->next;
    } while (a != aStart);

    qDebug() << "Exiting findIntersections(), found:" << found;
    return found;
}

void PolygonBoolean::classifyIntersections(std::vector<Vertex*>& subj, std::vector<Vertex*>& clip) {
    for (Vertex* v : subj) {
        if (v->isIntersection && !v->visited) {
            // Get mid-point before this intersection to use as test point
            QPointF mid = (v->prev->point + v->point) / 2;

            // Count crossings on clip polygon to determine inside/outside
            int count = 0;
            for (Vertex* c = clip[0];; c = c->next) {
                QPointF a = c->point;
                QPointF b = c->next->point;
                if (((a.y() > mid.y()) != (b.y() > mid.y())) &&
                    (mid.x() < (b.x() - a.x()) * (mid.y() - a.y()) / (b.y() - a.y()) + a.x())) {
                    count++;
                }
                if (c->next == clip[0]) break;
            }

            v->isEntry = (count % 2 == 0);  // Outside → Entry
            qDebug() << "Intersection at" << v->point << "classified as Entry=" << v->isEntry;
        }
    }
    for (Vertex* v : subj) {
    if (v->isIntersection)
        qDebug() << "Intersection:" << v->point
                 << " Entry:" << v->isEntry
                 << " Visited:" << v->visited;
}

}


std::vector<QPointF> PolygonBoolean::collectResult(std::vector<Vertex*>& subj) {
    std::vector<QPointF> result;

    for (Vertex* start : subj) {
        if (!start || !start->isIntersection || start->visited || !start->isEntry)
            continue;

        qDebug() << "Starting loop from intersection:" << start->point;

        Vertex* curr = start;
        int safety = 0;
        bool failed = false;

        do {
            if (!curr || safety++ > 10000) {
                qDebug() << "Aborting: loop safety triggered or null pointer.";
                failed = true;
                break;
            }

            qDebug() << "Visit" << curr->point;
            result.push_back(curr->point);
            curr->visited = true;

            // On intersection: jump to neighbor polygon
            if (curr->isIntersection && curr->neighbor && !curr->neighbor->visited) {
                curr = curr->neighbor;
                curr->visited = true;
                qDebug() << "Jump to neighbor:" << curr->point;
            }

            curr = curr->next;

        } while (curr && curr != start);

        if (!failed && !result.empty()) {
            qDebug() << "Collected loop with" << result.size() << "points.";
        
            qDebug() << "Candidate start vertex:" << start->point
                     << " isIntersection:" << start->isIntersection
                     << " isVisited:" << start->visited
                     << " isEntry:" << start->isEntry;
            break;  // Only one contour for now
        }
    }

    if (result.empty()) {
        qDebug() << "No valid intersection loop could be constructed.";
    }

    return result;
}



vector<QPointF> PolygonBoolean::intersect(const vector<QPointF>& poly1,
                                               const vector<QPointF>& poly2)
{
    auto subj = buildLinkedList(poly1);
    auto clip = buildLinkedList(poly2);

    if (!findIntersections(subj, clip)) return {}; // no intersection
    classifyIntersections(subj, clip);
    return collectResult(subj);
}


//UNION

vector<QPointF> PolygonBoolean::unionPolygons(const vector<QPointF>& poly1,
                                              const vector<QPointF>& poly2)
{
    auto subj = buildLinkedList(poly1);
    auto clip = buildLinkedList(poly2);

    if (!findIntersections(subj, clip)) {
        // No intersections → union is just both polygons together
        vector<QPointF> result = poly1;
        result.insert(result.end(), poly2.begin(), poly2.end());
        return result;
    }

    classifyIntersectionsForUnion(subj, clip);
    return collectUnionResult(subj);
}

void PolygonBoolean::classifyIntersectionsForUnion(std::vector<Vertex*>& subj, std::vector<Vertex*>& clip) {
    for (Vertex* v : subj) {
        if (v->isIntersection && !v->visited) {
            QPointF mid = (v->prev->point + v->point) / 2;

            int count = 0;
            for (Vertex* c = clip[0];; c = c->next) {
                QPointF a = c->point;
                QPointF b = c->next->point;
                if (((a.y() > mid.y()) != (b.y() > mid.y())) &&
                    (mid.x() < (b.x() - a.x()) * (mid.y() - a.y()) / (b.y() - a.y()) + a.x())) {
                    count++;
                }
                if (c->next == clip[0]) break;
            }

            // For UNION: Entry if we are inside the other polygon
            v->isEntry = (count % 2 == 1);
            qDebug() << "[Union] Intersection at" << v->point << "classified as Entry=" << v->isEntry;
        }
    }
}

std::vector<QPointF> PolygonBoolean::collectUnionResult(std::vector<Vertex*>& subj) {
    std::vector<QPointF> result;

    for (Vertex* start : subj) {
        if (!start || !start->isIntersection || start->visited)
            continue;

        // For debugging:
        qDebug() << "[Union] Candidate start vertex:" << start->point
                 << " isIntersection:" << start->isIntersection
                 << " isEntry:" << start->isEntry
                 << " visited:" << start->visited;

        Vertex* curr = start;
        std::vector<QPointF> tempResult;
        int safety = 0;

        do {
            if (!curr || safety++ > 10000) {
                qDebug() << "[Union] Loop broken due to safety limit.";
                tempResult.clear();
                break;
            }

            tempResult.push_back(curr->point);
            curr->visited = true;

            if (curr->isIntersection && curr->neighbor && !curr->neighbor->visited) {
                qDebug() << "[Union] Jumping to neighbor at:" << curr->neighbor->point;
                curr = curr->neighbor;
                curr->visited = true;
            }

            curr = curr->next;

        } while (curr && curr != start);

        if (!tempResult.empty()) {
            qDebug() << "[Union] Collected loop with" << tempResult.size() << " points.";
            result = tempResult;
            break;  // return first valid loop
        }
    }

    if (result.empty()) {
        qDebug() << "[Union] No valid union loop constructed.";
    }

    return result;
}

