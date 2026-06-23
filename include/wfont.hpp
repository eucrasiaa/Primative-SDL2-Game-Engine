#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct FontHeader {
  uint16_t numPerChar;
  uint16_t width;
  uint16_t height;
  uint16_t numChars;
};
#pragma pack(pop)

struct Font {
  static constexpr size_t BYTES_PER_CHAR = 18;
  FontHeader header = {0,0,0,0};
  // fontData = new uint8_t[totalFontSize];
  std::vector<uint8_t> dataBuffer;
  bool loadFromFile(const std::string& binaryFile) {
    std::ifstream file(binaryFile, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file.\n";
      return false;
    }
    file.seekg(0, std::ios::beg);
    uint8_t byte1, byte2;
    //amt of bits per each character
    file.read(reinterpret_cast<char*>(&byte1), 1);
    file.read(reinterpret_cast<char*>(&byte2), 1);
    header.numPerChar = (static_cast<uint16_t>(byte1) << 8) | byte2;

    // Read FONT_WIDTH (2 bytes)
    file.read(reinterpret_cast<char*>(&byte1), 1); 
    file.read(reinterpret_cast<char*>(&byte2), 1);
    header.width = (static_cast<uint16_t>(byte1) << 8) | byte2;

    // Read FONT_HEIGHT (2 bytes)
    file.read(reinterpret_cast<char*>(&byte1), 1); 
    file.read(reinterpret_cast<char*>(&byte2), 1);
    header.height = (static_cast<uint16_t>(byte1) << 8) | byte2;

    // Read NUM_CHARS_IN_FONT (2 bytes)
    file.read(reinterpret_cast<char*>(&byte1), 1); 
    file.read(reinterpret_cast<char*>(&byte2), 1);
    header.numChars = (static_cast<uint16_t>(byte1) << 8) | byte2;
    if (!file) {
      std::cerr << "Error: File is too short to contain a header.\n";
      return false;
    }
    std::cout << "Loaded Header Details:\n"
      << " - Num Per Char: " << header.numPerChar << "\n"
      << " - Width:        " << header.width << "\n"
      << " - Height:       " << header.height << "\n"
      << " - Total Chars:  " << header.numChars << "\n";
    signed long totalBytesToRead = static_cast<size_t>(header.numChars) * BYTES_PER_CHAR;
    dataBuffer.resize(totalBytesToRead);
    file.read(reinterpret_cast<char*>(dataBuffer.data()), totalBytesToRead);
    std::streamsize bytesRead = file.gcount();
    if (bytesRead < totalBytesToRead) {
      std::cout << "Warning: File ended early! Expected " << totalBytesToRead 
        << " bytes, but only read " << bytesRead << " bytes.\n";
    }

    return true;
  }

  void printCharacter(size_t charIndex) const {
    if (charIndex >= header.numChars) {
      std::cerr << "Error: Character index out of range.\n";
      return;
    }
    const uint8_t* charData = dataBuffer.data() + (charIndex * BYTES_PER_CHAR);
    std::cout << "\n--- Rendering Character Index: " << charIndex << " ---\n";

    for (int row = 0; row < header.height; ++row) {
      for (int col = 0; col < header.width; ++col) {
        int bitIndex = (row * header.width) + col;
        int byteOffset = bitIndex / 8;
        int bitPosition = 7 - (bitIndex % 8);
        uint8_t currentByte = charData[byteOffset];
        bool isPixelActive = (currentByte & (1 << bitPosition)) != 0;
        std::cout << (isPixelActive ? "##" : "  ");
      }
      std::cout << "\n";
    }
  }
  const uint8_t* getCharacterData(size_t charIndex) const {
    if (charIndex >= header.numChars) {
        return nullptr;
    }
    return dataBuffer.data() + (charIndex * BYTES_PER_CHAR);
  }
};
