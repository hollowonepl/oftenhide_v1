#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "../io/resource_file.h"

void show_syntax(char* execname)
{
    std::cout
        << "-----------------------------------------------" << std::endl 
        << "Syntax:" << std::endl
        << execname << " " << "[package_file] [options]" << std::endl
        << "Options:" << std::endl
        << "-c [files]      \t\t Create new package from files" << std::endl
        << "-x [directory]  \t\t Extract files into a directory"<< std::endl
        << "-l              \t\t List files inside the package"<< std::endl;
}

bool folder_exists(const char* path) 
{
    DIR* dir = opendir(path);
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
}

int ensure_paths(const char* relative_root, const char* path)
{

    std::string fullpath(relative_root);
    std::string filepath(path);

    if (folder_exists(fullpath.c_str())==false)
        return -1; //fail

    if (fullpath[fullpath.length()-1]!='/')
        fullpath+='/';

    std::string folder_path(fullpath);
    auto next_folder_marker = filepath.find_first_of("/");

    while (next_folder_marker!=std::string::npos)
    {
        folder_path+=filepath.substr(0,next_folder_marker);
        if (folder_exists(folder_path.c_str())==false)
        {
            mkdir(folder_path.c_str(), 0700);
            std::cout << "-- creating folder: " << folder_path << std::endl;
        }

        folder_path+="/";
        filepath = filepath.substr(next_folder_marker+1);
        next_folder_marker = filepath.find_first_of('/');
    }

    return 0;
}

int main(int argc, char** argv)
{
    ushine::io::resource_file resfile;
    
    std::cout
         << "OftenHide DemoGroup Resource Packager v1.0" << std::endl 
         << "coded by hollowone/OftenHide" << std::endl;

    if (argc<3)
    {
        show_syntax(argv[0]);
        exit(0);
    }

    if (strcmp(argv[2],"-c")==0)
    {
        if (argc<4)
        {
            show_syntax(argv[0]);
            exit(0);
        }

        std::cout
        << "-----------------------------------------------" << std::endl
        << "Creating new package file:" << std::endl; 

        int files_size = argc-3;
        char** files = new char*[files_size];

        for (auto i=0; i<files_size; i++)
        {
            files[i]=argv[i+3];
        }

        int status = resfile.create(argv[1], (char**)files,files_size );

        delete [] files;
        exit(0);
    }

    if (strcmp(argv[2],"-x")==0)
    {
        if (argc<4)
        {
            show_syntax(argv[0]);
            exit(0);
        }

        std::cout
        << "-----------------------------------------------" << std::endl
        << "Extracting from the package file:" << std::endl;    

        int status = resfile.open(argv[1]);

        if (status==0)
        {
            int files_count = resfile.get_files_count();
            if (files_count>0)
            {
                for (auto i=0; i<files_count; i++)
                {
                    char* path = NULL;
                    int size = 0;

                    resfile.get_file_header_by_idx(i, &path, &size);
                    
                    std::string search_path(path);

                    FILE* file_read = resfile.get_file_pointer_by_name(search_path.c_str());

                    std::cout << "- extracting : " << search_path << " size: " << size << " bytes." << std::endl;

                    if (file_read!=nullptr)
                    {
                        ensure_paths(argv[3], search_path.c_str());

                        FILE* file_save = fopen(search_path.c_str(),"wb");
                        if (file_save)
                        {
                            void* file_memory_block = malloc(size);

                            fread(file_memory_block,size,1,file_read);
                            fwrite(file_memory_block,size,1,file_save);
                            free(file_memory_block);
                            fclose(file_save);
                        } else status=-2; 
                    }  
                    else status=-1; 

                    if (status<0)
                    {
                        const char *error_type = (status==-1)?"read":"write";
                        std::cout<< "(error): could not " << error_type << " the path:" << search_path << std::endl;
                        status = 0;
                    }
                }
            }

            resfile.close();
        }


        exit(0);     
    }    

    if (strcmp(argv[2],"-l")==0)
    {
        std::cout
        << "-----------------------------------------------" << std::endl
        << "Listing of the package file:" << std::endl;     

        int status = resfile.open(argv[1]);
        if (status==0)
        {
            int files_count = resfile.get_files_count();
            if (files_count>0)
            {
                for (auto i=0; i<files_count; i++)
                {
                    char* path = NULL;
                    int size = 0;

                    resfile.get_file_header_by_idx(i, &path, &size);
                    std::cout << "- file: " << path << " size: " << size << " bytes." << std::endl;
                }
              

            }
        }
        exit(0);    
    }    

    show_syntax(argv[0]);

    return 0;
}