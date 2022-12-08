
#ifndef USHINE_IO_RESOURCE_MANAGER
#define USHINE_IO_RESOURCE_MANAGER

#include "../core/common.h"

namespace ushine
{

	namespace io
	{

    class resource_manager {

  	public:
  		resource_manager();
  		virtual ~resource_manager();

      int open_package(const char* path);
      int open_folder(const char* path);

      FILE* get_file_pointer_by_name(const char* search_path);

      int close();

    private:
      class pimpl;
  		std::unique_ptr<pimpl> impl;

    };

  }
}

#endif
