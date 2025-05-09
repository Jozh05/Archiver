#include "../header/App.hpp"

#ifdef _WIN32
#include <windows.h>

// Обработчик сигнала для Windows
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    if (fdwCtrlType == CTRL_C_EVENT) {
        std::cout << "\nCtrl+C pressed, stopping the app...\n";
        App::stopFlag = true;
        return TRUE;
    }
    return FALSE;
}

void App::setupSignalHandler() {
    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        std::cerr << "Failed to set control handler\n";
    }
}

#else
#include <csignal>

void App::signalHandler(int) {
    std::cout << "\nCtrl+C pressed, stopping the app...\n";
    App::stopFlag = true;
}

void App::setupSignalHandler() {
    std::signal(SIGINT, App::signalHandler);
}
#endif




void App::compressFile(const std::string& inputFile, const std::string& archiveName) {

    std::filesystem::path path(inputFile);
    std::filesystem::path outputPath = path.parent_path() / archiveName;

    arch = archive_write_new();
    if (!arch) {
        archive_write_free(arch);
        throw std::runtime_error("Failed to create archive");
    }
    archive_write_set_format_pax_restricted(arch);
    archive_write_add_filter_gzip(arch);

    if (archive_write_open_filename(arch, outputPath.generic_string().c_str()) != ARCHIVE_OK) {
        std::string errorMessage = archive_error_string(arch);
        archive_write_free(arch);
        throw std::runtime_error(errorMessage);
    }
    entry = archive_entry_new();
    checkSignal(OperationType::COMPRESS, outputPath);
    
    if (!entry) {
        archive_entry_free(entry);
        archive_write_free(arch);
        throw std::runtime_error("Failed to create archive entry");
    }
    
    std::ifstream file(inputFile, std::ios::binary);
    if (!file) {
        archive_entry_free(entry);
        archive_write_free(arch);
        throw std::runtime_error("Cannot open input file: " + inputFile);
    }

    checkSignal(OperationType::COMPRESS, outputPath);

    archive_entry_set_pathname(entry, path.filename().string().c_str());
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    archive_entry_set_size(entry, std::filesystem::file_size(path));

    checkSignal(OperationType::COMPRESS, outputPath);

    if (archive_write_header(arch, entry) != ARCHIVE_OK) {
        archive_entry_free(entry);
        archive_write_free(arch);
        throw std::runtime_error(archive_error_string(arch));
    }

    char buffer[32768];
    checkSignal(OperationType::COMPRESS, outputPath);
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
           
        checkSignal(OperationType::COMPRESS, outputPath);

        if (archive_write_data(arch, buffer, file.gcount()) < 0) {
            archive_entry_free(entry);
            archive_write_free(arch);
            throw std::runtime_error(archive_error_string(arch));
        }
    }

    archive_entry_free(entry);
    archive_write_close(arch);
    archive_write_free(arch);
}


void App::decompressFile(const std::string& archiveFile, const std::string& outputDir) {
    arch = archive_read_new();
    if (!arch) {
        archive_read_free(arch);
        throw std::runtime_error("Failed to create archive reader");
    }

    archive_read_support_format_all(arch);
    archive_read_support_filter_all(arch);


    if (archive_read_open_filename(arch, archiveFile.c_str(), 8192) != ARCHIVE_OK) {
        std::string errorMessage = archive_error_string(arch);
        archive_read_free(arch);
        throw std::runtime_error(errorMessage);
    }

    while (archive_read_next_header(arch, &entry) == ARCHIVE_OK) {
        std::string name = archive_entry_pathname(entry);
        std::filesystem::path outPath = std::filesystem::path(outputDir) / name;

        archive_entry_set_pathname(entry, outPath.generic_string().c_str());

        if (archive_read_extract(arch, entry, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM) != ARCHIVE_OK) {
            std::string errorMessage = archive_error_string(arch);
            archive_read_free(arch);
            throw std::runtime_error(errorMessage);
        }
        checkSignal(OperationType::DECOMPRESS, outPath);
    }

    archive_read_close(arch);
    archive_read_free(arch);
}

void App::checkSignal(OperationType operationType, const std::filesystem::path& outputPath) {

    switch (operationType)
    {
    case OperationType::COMPRESS:
        if (stopFlag) {
            archive_entry_free(entry);
            archive_write_free(arch);
            std::filesystem::remove(outputPath);
            throw std::runtime_error("Archiving was interrupted by the user");
        }
        break;
    case OperationType::DECOMPRESS:
        if (stopFlag) {
            archive_read_free(arch);
            std::filesystem::remove(outputPath);
            throw std::runtime_error("Extracting was interrupted by the user");
        }
    default:
        break;
    }
}

