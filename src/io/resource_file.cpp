#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "resource_file.h"
#include "../core/logger.h"

namespace ushine
{
	namespace io
	{
		struct package_source_file
		{
			char path[1024];
			int size;
			long file_pointer;
		};

		class resource_file::pimpl
		{
		public:
			FILE* file_handle;
			int file_mode;

			const static int FILE_STATUS_OK=0;
			const static int FILE_STATUS_FAIL=-1;	
			const static int FILE_STATUS_INVALID_HEADER=-2;				

			const static int OPEN_READ =0;
			const static int OPEN_WRITE=1;

			const char* HEADER_SIGNATURE = "OFHD";


			int 				 valid = FILE_STATUS_FAIL;
			int   				 files_count;
			package_source_file* files = NULL;
			long				 file_data_start_pos;

			int open_resource_file(const char* path, int mode)
			{

				switch(mode)
				{
					case OPEN_READ:
						file_handle = fopen(path, "rb");

						if (!file_handle)
							return FILE_STATUS_FAIL;

						return FILE_STATUS_OK;
					case OPEN_WRITE:
						file_handle = fopen(path, "wb");

						if (!file_handle)
							return FILE_STATUS_FAIL;

						return FILE_STATUS_OK;
				}

				return FILE_STATUS_FAIL;
			}

			int close_resource_file()
			{
				if (files!=NULL)
					delete [] files;

				if (file_handle)
					fclose(file_handle);
				
				return FILE_STATUS_OK;
			}

			int validate_source_files(char** files, int size)
			{
				for (auto file_idx=0; file_idx<size; file_idx++)
				{
					if( access( files[file_idx], F_OK ) == -1 ) 
						return file_idx;
				}
				return -1;
			}

			int get_file_size(char* path)
			{
				FILE* f = fopen(path,"rb");
				fseek(f, 0L, SEEK_END);
				int sz = ftell(f);
				fclose(f);
				return sz;
			}


			int save_resource_header(char** files, int size)
			{
				fwrite(HEADER_SIGNATURE,4,1,file_handle);
				fwrite(&size,sizeof(int),1,file_handle);

				for (auto file_idx=0; file_idx<size; file_idx++)
				{
					int path_len = strlen(files[file_idx]);
					fwrite(files[file_idx],path_len,1,file_handle);
					unsigned char zero=0;
					fwrite(&zero,1,1,file_handle);
					unsigned int file_size = get_file_size(files[file_idx]);
					fwrite(&file_size,sizeof(unsigned int),1,file_handle);
				}

				return FILE_STATUS_OK;
			}

			int add_file(char* srcfile)
			{
				FILE* f = fopen(srcfile,"rb");


				fseek(f, 0L, SEEK_END);
				int sz = ftell(f);
				fseek(f, 0L, SEEK_SET);

				void *buf = (void*)malloc(sz);

				fread(buf,sz,1,f);
				fwrite(buf,sz,1,file_handle);

				free(buf);
				fclose(f);

				return FILE_STATUS_OK;
			}

			int check_package_header()
			{
				fseek(file_handle, 0L, SEEK_SET);
				char signature[5] = {0,0,0,0,0};

				fread(signature,4,1,file_handle);
				
				if (strcmp(signature,HEADER_SIGNATURE)!=0)
				{
					valid = FILE_STATUS_INVALID_HEADER;
					return valid;
				}

				fread(&files_count,sizeof(int),1,file_handle);

				if (files_count<=0)
				{
					valid = FILE_STATUS_INVALID_HEADER;
					return valid;
				}

				files = new package_source_file[files_count];

				for (auto i=0; i<files_count; i++)
				{
					auto current_pos = ftell(file_handle);
					fgets(files[i].path,1024,file_handle);

					auto pathlen = strlen(files[i].path);
					fseek(file_handle,current_pos+pathlen+1,SEEK_SET);

					fread(&files[i].size,4,1,file_handle);
				}

				file_data_start_pos = ftell(file_handle);

				long current_pos = file_data_start_pos;
				for (auto i=0; i<files_count; i++)
				{
					files[i].file_pointer = current_pos;
					current_pos+=files[i].size;
				}

				valid=FILE_STATUS_OK;
				return valid;
			}



		};

		resource_file::resource_file() : impl(new pimpl)
		{

		}
		resource_file::~resource_file()
		{

		}

		int resource_file::open(const char* filename)
		{
			auto status =  impl->open_resource_file(filename, impl->OPEN_READ);
			if (status!=impl->FILE_STATUS_OK)
				return status;

			status = impl->check_package_header();

			//std::cout << "check_package_header status = " << status << std::endl;

			if (status!=impl->FILE_STATUS_OK)
				return status;

			return impl->FILE_STATUS_OK;
		}

		int resource_file::get_files_count(void)
		{
			if (impl->valid==impl->FILE_STATUS_OK)
				return impl->files_count;
			
			return impl->FILE_STATUS_FAIL;
		}

		int resource_file::get_file_header_by_idx(int file_idx, char** name, int* size)
		{
			if (name==nullptr)
				return impl->FILE_STATUS_FAIL;

			if (impl->valid==impl->FILE_STATUS_OK)
			{

				auto file_struct = impl->files[file_idx];
				*name = file_struct.path;

				if (size!=nullptr)
					*size = file_struct.size;

				return impl->FILE_STATUS_OK;
			}
			return impl->FILE_STATUS_FAIL;
		}	

		FILE* resource_file::get_file_pointer_by_idx(int file_idx)
		{
			if (impl->valid==impl->FILE_STATUS_OK)
			{
				auto status = fseek(impl->file_handle, impl->files[file_idx].file_pointer, SEEK_SET);
				if (status==0) return impl->file_handle;
			}
			return nullptr;
		}


		FILE* resource_file::get_file_pointer_by_name(const char* search_path)
		{
			if (search_path==nullptr)
				return nullptr;

			if (impl->valid!=impl->FILE_STATUS_OK)
				return nullptr;

			for (auto i=0; i<impl->files_count; i++)
			{
				if (strcmp(impl->files[i].path,search_path)==0)
				{
					return get_file_pointer_by_idx(i);
				}	
			}
			return nullptr;
		}


		int resource_file::close()
		{
			return impl->close_resource_file();
		}

      	int resource_file::create(const char* filename, char** srcfiles, int srcfiles_size)
		{
			auto status = impl->validate_source_files(srcfiles, srcfiles_size);
			if (status!=-1)
			{
				std::cout << "ERROR(PACKAGE_MANAGER): Failed to find source file: " << srcfiles[status] << std::endl;

				return impl->FILE_STATUS_FAIL;
			}

			status = impl->open_resource_file(filename, impl->OPEN_WRITE);

			if (status==impl->FILE_STATUS_FAIL)
				return status;

			impl->save_resource_header(srcfiles,srcfiles_size);

			for (auto file_idx=0; file_idx<srcfiles_size; file_idx++)
			{
				std::cout << "- adding file: " << srcfiles[file_idx] << std::endl;
				impl->add_file(srcfiles[file_idx]);
			}

			return impl->close_resource_file();
		}

		



  }
}
