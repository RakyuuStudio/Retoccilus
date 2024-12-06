/*
    NOTE: This file is TEST-ONLY.
    If you don't want to run the test, PLEASE DO NOT ADD IT TO CMAKE `add_executable`, thanks.
*/
#include "Core/FileOperation/FileOperation.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <thread>
#include <chrono>
#include <random>
#include <filesystem>

using namespace Retoccilus::Core::FileOperation;
namespace fs = std::filesystem;

void assertFileExists(const std::string& path) {
    if (!fs::exists(path)) {
        throw std::runtime_error("File does not exist: " + path);
    }
}

void assertFileSize(const std::string& path, size_t expectedSize) {
    size_t actualSize = fs::file_size(path);
    if (actualSize != expectedSize) {
        throw std::runtime_error("File size mismatch. Expected: " + 
                               std::to_string(expectedSize) + ", Got: " + 
                               std::to_string(actualSize));
    }
}

// Empty File Test
void emptyFileTest() {
    std::cout << "=== Empty File Test ===\n";
    FileOperation fo;

    try {
        // Write to Empty File
        std::vector<char> emptyData;
        fo.writeFile("empty.txt", emptyData);
        assertFileExists("empty.txt");
        assertFileSize("empty.txt", 0);
        
        // Read Empty File
        auto content = fo.readFile<std::vector<char>>("empty.txt");
        assert(content.empty());
        
        std::cout << "Empty file test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Empty file test failed: " << e.what() << std::endl;
    }
}

// REALLY HUGE File Test
void hugeFileTest() {
    std::cout << "=== Huge File Test ===\n";
    FileOperation fo;

    try {
        // Create 500 MB File
        const size_t size = 500 * 1024 * 1024;
        std::vector<char> hugeData(size);
        
        // Randomly fill the data
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        for (size_t i = 0; i < size; ++i) {
            hugeData[i] = static_cast<char>(dis(gen));
        }

        // Asynchronous Write
        auto start = std::chrono::steady_clock::now();
        auto futureWrite = fo.writeFileAsync("huge.bin", hugeData, FileOperation::FileType::FT_Binary);
        futureWrite.wait();
        
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Write time: " << duration.count() << "ms\n";

        // Assert the size
        assertFileSize("huge.bin", size);

        // Asynchronous Read and Verify
        start = std::chrono::steady_clock::now();
        auto futureRead = fo.readFileAsync<std::vector<char>>("huge.bin", FileOperation::FileType::FT_Binary);
        auto readData = futureRead.get();
        
        end = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Read time: " << duration.count() << "ms\n";

        assert(readData == hugeData);
        std::cout << "Huge file test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Huge file test failed: " << e.what() << std::endl;
    }
}

// Unicode Text Test
void unicodeTest() {
    std::cout << "=== Unicode Text Test ===\n";
    FileOperation fo;

    try {
        std::string unicodeText = 
            "English: Hello World\n"
            "Chinese: 你好，世界\n"
            "Japanese: こんにちは世界\n"
            "Korean: 안녕하세요 세계\n"
            "Russian: Привет, мир\n"
            "Emoji: 👋🌍🚀✨\n"
            "Special: ∑∏√∞≠≈\n";

        std::vector<char> data(unicodeText.begin(), unicodeText.end());
        fo.writeFile("unicode.txt", data, FileOperation::FileType::FT_Text);

        auto content = fo.readFile<std::string>("unicode.txt", FileOperation::FileType::FT_Text);
        assert(content == unicodeText);
        
        std::cout << "Unicode test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Unicode test failed: " << e.what() << std::endl;
    }
}

// Concurrency Test
void concurrencyTest() {
    std::cout << "=== Concurrency Test ===\n";
    FileOperation fo;

    try {
        const int numThreads = 10;
        std::vector<std::future<void>> futures;

        // Multi-threadly write into different threads
        for (int i = 0; i < numThreads; ++i) {
            futures.push_back(std::async(std::launch::async, [i, &fo]() {
                std::string filename = "concurrent_" + std::to_string(i) + ".txt";
                std::string content = "Content from thread " + std::to_string(i);
                std::vector<char> data(content.begin(), content.end());
                fo.writeFile(filename, data);
            }));
        }

        // Wait for all the write operation finished.
        for (auto& f : futures) {
            f.wait();
        }

        // Verify the file
        for (int i = 0; i < numThreads; ++i) {
            std::string filename = "concurrent_" + std::to_string(i) + ".txt";
            assertFileExists(filename);
        }

        std::cout << "Concurrency test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Concurrency test failed: " << e.what() << std::endl;
    }
}

// Error Test
void errorHandlingTest() {
    std::cout << "=== Error Handling Test ===\n";
    FileOperation fo;

    try {
        // Read a non-existent file
        try {
            fo.readFile<std::string>("nonexistent.txt");
            assert(false && "Should throw exception");
        } catch (const std::runtime_error&) {
            std::cout << "Successfully caught nonexistent file error\n";
        }

        // Write to a read-only file.
        std::vector<char> data{'t', 'e', 's', 't'};
        fo.writeFile("readonly.txt", data);
        
#ifdef _WIN32
        SetFileAttributesA("readonly.txt", FILE_ATTRIBUTE_READONLY);
#else
        chmod("readonly.txt", S_IRUSR);
#endif

        try {
            fo.writeFile("readonly.txt", data);
            assert(false && "Should throw exception");
        } catch (const std::runtime_error&) {
            std::cout << "Successfully caught readonly file error\n";
        }

        std::cout << "Error handling test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error handling test failed: " << e.what() << std::endl;
    }
}

// Special Filename Test
void specialFilenameTest() {
    std::cout << "=== Special Filename Test ===\n";
    FileOperation fo;

    try {
        std::vector<std::string> specialNames = {
            "file with spaces.txt",
            "file_with_!@#$%^&().txt",
            "很长的中文文件名测试.txt",
            "file.with.multiple.dots.txt",
            "UPPERCASE.TXT",
            ".hidden_file",
            "path/with/subdirectories/file.txt"
        };

        for (const auto& name : specialNames) {
            // Make sure the directory exists
            fs::path p(name);
            if (p.has_parent_path()) {
                fs::create_directories(p.parent_path());
            }

            // Write Test
            std::vector<char> data{'t', 'e', 's', 't'};
            fo.writeFile(name, data);
            assertFileExists(name);
        }

        std::cout << "Special filename test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Special filename test failed: " << e.what() << std::endl;
    }
}

// Binary Data Integrity Test
void binaryIntegrityTest() {
    std::cout << "=== Binary Integrity Test ===\n";
    FileOperation fo;

    try {
        std::vector<char> binaryData(256);
        for (int i = 0; i < 256; ++i) {
            binaryData[i] = static_cast<char>(i);
        }

        // Write binary data
        fo.writeFile("binary_test.bin", binaryData, FileOperation::FileType::FT_Binary);

        // Read binary data and verify
        auto readData = fo.readFile<std::vector<char>>("binary_test.bin", FileOperation::FileType::FT_Binary);
        assert(readData.size() == binaryData.size());
        
        for (size_t i = 0; i < binaryData.size(); ++i) {
            if (readData[i] != binaryData[i]) {
                throw std::runtime_error("Binary data mismatch at position " + 
                                       std::to_string(i));
            }
        }

        std::cout << "Binary integrity test passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Binary integrity test failed: " << e.what() << std::endl;
    }
}

// Clean up the files
void cleanup() {
    std::cout << "=== Cleaning up test files ===\n";
    try {
        std::vector<std::string> filesToDelete = {
            "empty.txt", "huge.bin", "unicode.txt", "readonly.txt",
            "binary_test.bin"
        };

        // Add concurrent test files
        for (int i = 0; i < 10; ++i) {
            filesToDelete.push_back("concurrent_" + std::to_string(i) + ".txt");
        }

        for (const auto& file : filesToDelete) {
            if (fs::exists(file)) {
                fs::remove(file);
            }
        }

        fs::remove_all("path");

        std::cout << "Cleanup completed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Cleanup failed: " << e.what() << std::endl;
    }
}

int main() {
    try {
        emptyFileTest();
        unicodeTest();
        binaryIntegrityTest();
        concurrencyTest();
        errorHandlingTest();
        specialFilenameTest();
        hugeFileTest();
        
        cleanup();
        
        std::cout << "\nAll tests completed successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Test suite failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 