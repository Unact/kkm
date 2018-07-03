# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

LIBDIR     = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']

HEADER_DIRS = [INCLUDEDIR, File.expand_path(File.join(File.dirname(__FILE__), "lib/include"))]
# setup constant that is equal to that of the file path that holds that static libraries that will need to be compiled against
LIB_DIRS = [LIBDIR, File.expand_path(File.join(File.dirname(__FILE__), "lib/bins"))]

# array of all libraries that the C extension should be compiled against
libs = ['-lfptr']

# iterate though the libs array, and append them to the $LOCAL_LIBS array used for the makefile creation
libs.each do |lib|
  $LOCAL_LIBS << "#{lib} "
end

# Give it a name
extension_name = 'kkm'

# The destination
dir_config(extension_name, HEADER_DIRS, LIB_DIRS)

# Do the work
create_makefile("#{extension_name}/#{extension_name}")
