#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "resource_file.h"

#include "resource_manager.h"
#include "../core/logger.h"

namespace ushine
{
	namespace io
	{
		class resource_manager::pimpl
		{
		public:

		};

		resource_manager::resource_manager() : impl(new pimpl)
		{

		}
		resource_manager::~resource_manager()
		{

		}

      	int resource_manager::open_package(const char* path)
		{
			return -1;
		}
      
	  	int resource_manager::open_folder(const char* path)
		{
			return -1;
		}

      	FILE* resource_manager::get_file_pointer_by_name(const char* search_path)
		{
			return nullptr;
		}

      	int resource_manager::close()
		{
			return -1;
		}


  }
}
