//
//  main.cpp
//
//  undupe
//
//  DESCRIPTION:
//  undupe is a simple command line c++ program that takes image files in one directory,
//      hashes them, compares the hashes to each other to see if they are hash exact,
//      then moves dupes out of the original directory to a new one.
//
//  Compile in XCode, use g++ or gcc
//
//  Created by Jett Sjöberg on 6/15/23.
//

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

// Hasher (THIS IS NOT A CRYPTO SECURE HASH)
std::string generateHash(const std::string& filePath) {
  std::hash<std::string> hasher;
  std::ifstream file(filePath, std::ios::binary);

  std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return std::to_string(hasher(fileContents));
}

// File mover
void moveFile(const std::string& filePath, const std::string& outputDir) {
  fs::path sourcePath(filePath);
  fs::path outputPath(outputDir);

  // Create output directory if it doesn't exist
  fs::create_directories(outputPath);

  fs::path destination = outputPath / sourcePath.filename();

  fs::rename(sourcePath, destination);
  std::cout << "Moved file: " << destination << std::endl;
}

int main(int argc, const char* argv[]) {
  std::string inputDir;
  std::string outputDir;

  // Parse command line argument
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-i" && i + 1 < argc) {
      inputDir = argv[i + 1];
    } else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
      outputDir = argv[i + 1];
    }
  }

  // If nothing or improper arguments on command line, output help.
  if (inputDir.empty() || outputDir.empty()) {
    std::cerr << "Usage: dupe -i <input directory> -o <output directory>" << std::endl;
    return 1;
  }

  // Store hashes and files
  std::unordered_map<std::string, std::vector<std::string>> hashToFileMap;

  // Generate hashes for image files in input directory
  for (const auto& entry : fs::recursive_directory_iterator(inputDir)) {
    if (!entry.is_regular_file()) {
      continue;
    }

    std::string filePath = entry.path().string();
    // Check if file type is image
    if (
        filePath.ends_with(".jpg") ||
        filePath.ends_with(".png") ||
        filePath.ends_with(".gif") ||
        filePath.ends_with(".webp") ||
        filePath.ends_with(".bmp")  ||
        filePath.ends_with(".jpeg") ||
        filePath.ends_with(".tiff") ||
        filePath.ends_with(".heif")) {
      std::string fileHash = generateHash(filePath);
      hashToFileMap[fileHash].push_back(filePath);
    }
  }

  // Find duplicate hashes
  std::vector<std::pair<std::string, std::vector<std::string>>> duplicates;
  for (const auto& pair : hashToFileMap) {
    if (pair.second.size() > 1) {
      duplicates.push_back(pair);
    }
  }

  // Output all duplicate hashes and filenames
  if (!duplicates.empty()) {
    for (const auto& duplicate : duplicates) {
      std::cout << duplicate.first << ":";
      for (const auto& filePath : duplicate.second) {
        std::cout << " " << filePath;
      }
      std::cout << std::endl;
    }

    // Ask user y/n
    std::cout << "Move the duplicates to output directory? (y/n): ";
    std::string userInput;
    std::cin >> userInput;

    if (userInput == "y") {
      // Move duplicate files
      for (const auto& duplicate : duplicates) {
        for (size_t i = 1; i < duplicate.second.size(); ++i) {
          moveFile(duplicate.second[i], outputDir);
        }
      }
    }
  }

  return 0;
}
