#include <archive.h>
#include <archive_entry.h>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>


void compressFile(const std::string& inputFile, const std::string& archiveName) {

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

    archive_entry_free(entry);
    archive_write_close(arch);
    archive_write_free(arch);
}


void decompressFile(const std::string& archiveFile, const std::string& outputDir) {

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

    archive_read_close(arch);
    archive_read_free(arch);
}


int main(int argc, char** argv) {
    
    if (argc != 3){
        std::cout << "using: archiver <File path> <Archive path>" << std::endl;
        return 1;
    }
    
    compressFile(argv[1], argv[2]);
    decompressFile(argv[2], "./data");

    return 0;
}