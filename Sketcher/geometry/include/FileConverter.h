#ifndef FILE_CONVERTER_H
#define FILE_CONVERTER_H

#include <string>
#include <vector>
#include <array>
#include <utility>  // For std::pair

class FileConverter {
public:
    void convertObjToStl(const std::string& objFilename, const std::string& stlFilename);
    void convertStlToDat(const std::string& stlFilename, const std::string& datFilename);
    std::pair<std::vector<std::vector<double>>, std::vector<std::array<int, 3>>> 
    load(const std::string& filePath); // Updated declaration
    void save(const std::vector<std::vector<double>>& vertices, const std::string& filePath);

private:
    std::pair<std::vector<std::array<double, 3>>, std::vector<std::array<int, 3>>> 
    readObj(const std::string& filename);

    void writeStl(const std::vector<std::array<double, 3>>& vertices, 
                  const std::vector<std::array<int, 3>>& faces, 
                  const std::string& filename);

    std::vector<std::array<double, 3>> readStl(const std::string& filename);

    void writeDat(const std::vector<std::array<double, 3>>& vertices, 
                  const std::string& filename);
};

#endif