#include "../header/App.hpp"

void App::compressFile(const std::string& inputFile, const std::string& archiveName) {

    archive* arch = archive_write_new();
    if (!arch) 
        throw std::runtime_error("Failed to create archive");

    archive_write_set_format_pax_restricted(arch);
    archive_write_add_filter_gzip(arch);

    if (archive_write_open_filename(arch, archiveName.c_str()) != ARCHIVE_OK)
        throw std::runtime_error(archive_error_string(arch));

    archive_entry* entry = archive_entry_new();
    
    if (!entry)
        throw std::runtime_error("Failed to create archive entry");
    
    std::ifstream file(inputFile, std::ios::binary);
    if (!file) 
        throw std::runtime_error("Cannot open input file: " + inputFile);

    std::filesystem::path path(inputFile);

    archive_entry_set_pathname(entry, path.generic_string().c_str());
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    archive_entry_set_size(entry, std::filesystem::file_size(path));

    if (archive_write_header(arch, entry) != ARCHIVE_OK) 
        throw std::runtime_error(archive_error_string(arch));

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        if (archive_write_data(arch, buffer, file.gcount()) < 0)
            throw std::runtime_error(archive_error_string(arch));
    }


    free(OperationType::COMPRESS, arch, entry);
}




void App::decompressFile(const std::string& archiveFile, const std::string& outputDir) {
    archive* arch = archive_read_new();
    if (!arch)
        throw std::runtime_error("Failed to create archive reader");

    archive_read_support_format_all(arch);
    archive_read_support_filter_all(arch);

    if (archive_read_open_filename(arch, archiveFile.c_str(), 8192) != ARCHIVE_OK)
        throw std::runtime_error(archive_error_string(arch));

    archive_entry* entry;

    while (archive_read_next_header(arch, &entry) == ARCHIVE_OK) {
        std::string name = archive_entry_pathname(entry);
        std::filesystem::path outPath = std::filesystem::path(outputDir) / name;

        archive_entry_set_pathname(entry, outPath.generic_string().c_str());

        if (archive_read_extract(arch, entry, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM) != ARCHIVE_OK)
            throw std::runtime_error(archive_error_string(arch));
    }

    free(OperationType::DECOMPRESS, arch, entry);
}



void App::free(OperationType operationType, archive* arch, archive_entry* entry) {

    switch (operationType)
    {
    case OperationType::COMPRESS:
        archive_entry_free(entry);
        archive_write_close(arch);
        archive_write_free(arch);
        break;
    
    case OperationType::DECOMPRESS:
        archive_read_close(arch);
        archive_read_free(arch);
        break;
    default:
        break;
    }
}



bool App::validateLine(int argc, char* arg0) {
    
    if (argc != 4){
        std::cout << "Using:" << std::endl;
        std::cout << arg0 << " cmp <File path> <Archive name> to compress file" << std::endl;
        std::cout << arg0 << " dcmp <Archive path> <Output directory> to decompress file" << std::endl;
        return false;
    }
    return true;
}



OperationType App::validateCommand(char* command) {

    OperationType operationType;
    if (command == "cmp") {
        operationType = OperationType::COMPRESS;
    }
    else if (command == "dcmp") {
        operationType = OperationType::DECOMPRESS;
    }
    else {
        throw std::runtime_error("Incorrect command");
    }
    return operationType;
}



std::string App::validateStringAsFilePath(char* pathStr) {
    std::error_code ec;
    std::filesystem::path path(pathStr);
    if (std::filesystem::exists(path, ec) && !ec && std::filesystem::is_regular_file(path, ec) && !ec)
        return std::string(pathStr);
    else
        throw std::invalid_argument("Incorrect path to file");
}

std::string App::validateStringAsFileName(char* fileName) {
    
    std::string result(fileName);
    const std::string invalid = R"(/\:*?"<>|)";

    for (char c : result) {
        if (invalid.find(c) != std::string::npos) {
            throw std::invalid_argument("Invalid file name");
        }
    }
    return result;
}

std::string App::validateStringAsDirName(char* dirName) {
    std::string result(dirName);
    const std::string invalid = R"(:*?"<>|)";

    for (char c : result) {
        if (invalid.find(c) != std::string::npos) {
            throw std::invalid_argument("Invalid file name");
        }
    }
    return result;
}

std::pair<std::string, std::string> App::validateArgs(char* arg1, char* arg2, OperationType operationType) {
    switch (operationType)
    {
    case OperationType::COMPRESS: {
        std::string inputFile = validateStringAsFilePath(arg1);
        std::string archiveName = validateStringAsFileName(arg2);
        return {inputFile, archiveName};
    }
    case OperationType::DECOMPRESS: {
        std::string archivePath = validateStringAsFilePath(arg1);
        std::string outputDir = validateStringAsDirName(arg2);
        return {archivePath, outputDir};
    }
    default:
        break;
    }
}



int App::execute(int argc, char** argv) {


}