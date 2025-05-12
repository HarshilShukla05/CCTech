#ifndef POLYGON_BOOLEAN_H
#define POLYGON_BOOLEAN_H

#include <QPointF>
#include <vector>

class PolygonBoolean {
public:
    static std::vector<QPointF> intersect(
        const std::vector<QPointF>& subject,
        const std::vector<QPointF>& clip);

    static std::vector<QPointF> unionPolygons(
        const std::vector<QPointF>& subject,
        const std::vector<QPointF>& clip);

    struct GHVertex {
        QPointF point;
        bool isIntersection = false;
        bool isEntry = false;
        bool visited = false;
        GHVertex* neighbor = nullptr;
        GHVertex* next = nullptr;
        GHVertex* prev = nullptr;

        GHVertex(QPointF pt) : point(pt) {}
    };

private:
    static std::vector<GHVertex*> buildLinkedList(const std::vector<QPointF>& polygon);
    static bool findIntersections(std::vector<GHVertex*>& subjList, std::vector<GHVertex*>& clipList);
    static bool segmentsIntersect(const QPointF& a1, const QPointF& a2,
                                  const QPointF& b1, const QPointF& b2,
                                  QPointF& intersection);

    static void classifyIntersections(std::vector<GHVertex*>& subjList,
                                      std::vector<GHVertex*>& clipList);
    static void classifyIntersectionsForUnion(std::vector<GHVertex*>& subjList,
                                              std::vector<GHVertex*>& clipList);

    static std::vector<QPointF> collectResult(std::vector<GHVertex*>& subjList);
    static std::vector<QPointF> collectUnionResult(std::vector<GHVertex*>& subjList);

        static void resetVisitedFlags(std::vector<GHVertex*>& vertices);
};

#endif // POLYGON_BOOLEAN_H
