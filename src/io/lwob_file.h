
#ifndef USHINE_IO_LWOBFILE
#define USHINE_IO_LWOBFILE

#include "../core/common.h"

namespace ushine
{

	namespace io
	{

    class lwob_file {

  	public:
  		lwob_file();
  		virtual ~lwob_file();

      int open(const char* filename);
      int close();

      int read_all();

      int show_stats();

    private:
      class pimpl;
  		std::unique_ptr<pimpl> impl;

    };

  }
}

#endif
