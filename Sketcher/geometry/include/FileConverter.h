#ifndef FILE_CONVERTER_H
#define FILE_CONVERTER_H

#include <string>
#include <vector>
#include <utility>  // For std::pair

class FileConverter {
public:
    void convertObjToStl(const std::string& objFilename, const std::string& stlFilename);
    void convertStlToDat(const std::string& stlFilename, const std::string& datFilename);

private:
    std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> 
    readObj(const std::string& filename);

    void writeStl(const std::vector<std::vector<double>>& vertices, 
                  const std::vector<std::vector<int>>& faces, 
                  const std::string& filename);

    std::vector<std::vector<double>> readStl(const std::string& filename);

    void writeDat(const std::vector<std::vector<double>>& vertices, 
                  const std::string& filename);
};

#endif
