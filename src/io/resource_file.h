
#ifndef USHINE_IO_RESOURCE_FILE
#define USHINE_IO_RESOURCE_FILE

#include "../core/common.h"

namespace ushine
{

	namespace io
	{

    class resource_file {

  	public:
  		resource_file();
  		virtual ~resource_file();

      int open(const char* filename);
      int create(const char* filename, char** srcfiles, int srcfiles_size);

      int get_files_count(void);
      int get_file_header_by_idx(int file_idx, char** name, int* size=NULL);
      FILE* get_file_pointer_by_idx(int file_idx);
      FILE* get_file_pointer_by_name(const char* search_path);

      int close();

    private:
      class pimpl;
  		std::unique_ptr<pimpl> impl;

    };

  }
}

#endif
