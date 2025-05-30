Hi!

The small sections are:

0.  Ultra quick instructions
1.  Introduction
2.  Notes about the package
3.  Compilation and installation
4.  Using the library to compile your program
5.  Linux/UNIX dynamic libraries
6.  Libraries and tools needed
7.  Examples
8.  Dialog editor
9.  Targets supported, limitations
10. CPU usage
11. Notes about international support
12. How to submit a patch
13. Memory Debuggers
14. Special thanks
15. Contact information

  The instructions to compile the source code can be found in the INSTALL.txt
file. I strongly suggest reading the INSTALL.txt file and the files indicated
by it.

0.  Ultra quick instructions
----------------------------

  If you can't wait even a second and you are running a UNIX like system with
most of the GNU tools (Perl, shell utils, file utils, bash, make, etc.) you
can just follow these steps: [These tools are installed in all Linux systems;
they are available for most UNIX systems and also for djgpp, Cygwin and MinGW]

A) Run the configure script (./configure or configure).

Examples:
   linux:/usr/src/tvision# ./configure
   c:\djgpp\contrib\tvision>configure

B) If all is OK run "make".
C) If all is OK just run "make install". [On UNIX systems you'll need to
change to the root user before installing]

  With it you'll get the headers and library installed.
  If the installation prefix directory is wrongly guessed call the configure
script passing --prefix=dir as argument.
Example:
  linux:/usr/src/tvision# ./configure --prefix=/usr/local
  
  Note: The configuration mechanism can be used for Cygwin or MinGW but you
must install some GNU tools. It can't be used for Borland and Microsoft
compilers.




1. Introduction:
---------------

  That's the Turbo Vision distribution (TV for short) v2.1.0.
  The sources are copyrighted by Borland (currently Inprise) and are freely
available in internet from:

ftp://ftp.inprise.com/pub/borlandcpp/devsupport/archive/turbovision/tv.zip

  Look in the borland.txt file for more information. Additionally here is
what Inprise says about this package:

URL: http://www.inprise.com/devsupport/bcppbuilder/faq/QNA906.html
or:  http://community.borland.com/article/0,1410,17285,00.html
------------------------------------------------------------------------------
Question and Answer Database

QNA        :Turbo Vision
Category   :C/C++ Language Issues
Platform   :Windows95
Product   :C++Builder  ALL

Question:
Where can I find the public domain version of Turbo Vision?

Answer:
It can be found at
ftp.inprise.com/pub/borlandcpp/devsupport/archive/turbovision/

the turbo vision libraries are in the tv.zip archve, the rest of the 
files are examples.

7/2/1998 10:32:32 AM
------------------------------------------------------------------------------
  So according to Inprise web tv.zip is a Public Domain version.
<Clarification>
  I added this small text because some people have doubts about copyrights
issues.
</Clarification>

  They were heavily patched to work with gcc, BC++ and MSVC. Some patches
fixes bugs, others changes the behavior of things limited to 16 bits values
and others add brand new functionality. The resulting TV isn't 100%
compatible with the original. See the examples/porting.txt file for more
information, I explain which differences I found while porting the examples.
  Up to release 1.0.1 most of the patches were made by Robert Hoehne. I
contributed with various patches and modules mainly to extend the original
functionality. For information about what changed before it just read the
change.log and change1.log files. You'll find more information in the "doc"
directory.
  The library is distributed under the GPL license, it seems to be OK because
according to a FAQ in the Inprise web site the base library is Public
Domain. The file "copying" contains the GPL license and the file "copying.rh"
contains the RHIDE copyright. Some portions of the contributed code are under
the BSD license (QNX RtP driver). I took some code from the Sigala's port of
TV (some examples), Sigala's port is under the BSD license, but I modified
the code and my changes are covered by the GPL license. If you want the
code under BSD look for Sigala's port. I also added some code from TV Tools,
this package was created by Marc Stern <stern@mble.philips.be> and he states:
"You can freely distribute them, modify them and use them for any purpose
(even include it into commercial products).". For the purpose of this
distribution the code is under GPL license.

  Recent news: Richard M. Stallman contacted me and put some doubts about the
distribution of Borland's sources, he contacted somebody at Borland asking if
TV was freesoftware and he got no as answer. I think this reply was mainly
because RMS asked without clarifying your point. To clarify it I contacted
the person in charge of the Linux web section of Borland and he told me that
he is redirecting these doubts to the right people inside Borland. Until now
I didn't get a final answer. Note it was in 2000, Borland never replied and I
insisted during 18 months. Also note that I tried to contact Linux
International but they never replied.




2. Notes about the package:
--------------------------

  As you are reading it you already unpacked it but here are some notes:
(I usually read the readmes without unpacking)
  The distribution is done using two formats: .zip and .tar.gz. The .zip file
contains the files in "DOS" format (\r\n as end of line) and the .tar.gz file
contains the same files in "UNIX" format (\n as end of line). The .zip file
also lacks a few files needed to create Debian packages because these files
have names that can't be safetly used in environments where the file names
are limited to 8+3 characters.
  If you use the .tar.gz file for a system like djgpp (DOS) you can use the
file lista.dos to know which files should be converted to DOS format. The
script called fromunix.pl converts those files. The rest of the file doesn't
usually need any change because gcc understands both kind of end of line
terminators. I don't know if bcc and msvc also supports it.
  Note that sometimes I talk about "Linux" but it usually applies to most
POSIX systems. Also note that Cygwin is more similar to Linux than DOS.
  Starting with 1.0.5 I include the Debian package files. As these files
aren't 8+3 clean if you uncompress the files in plain DOS you'll get some
warnings about files with the same name, so I removed these files from the
DOS distribution in 1.0.6, now they are only in the Linux distribution. So
if you want to try generating the .deb packages you *must* have the Linux
distribution of the library.
  The compress and compress.bat files are used to create the distribution
packages and are included. If you need to generate the .zip file just use the
files in the djgpp directory. If you need to generate the .tar.gz just use
the files in the linux directory.
  Starting with 1.1.0 Win32 platforms are also supported using MingW and BC++
5.5 compilers.
  In 2.0.0 we added MSVC and QNX support. The UNIX support was also enhanced
to support FreeBSD and Solaris.
  Starting with 2.0.2 the make process is unified and the resulting library
is generated in the makes directory for all architectures. Currently the only
target that isn't integrated is MSVC because Microsoft doesn't provide a free
compiler like Borland does. I need volunteers to integrate the MSVC to the
rest and make things easier for MSVC users. For more information read the
INSTALL.txt file.




3. Compilation and installation:
-------------------------------

  The compilation instructions are in the INSTALL.txt file.
  All the .o/.obj files are stored in the makes/obj directory it helps to
delete or compress these files if you want to save disk space and you won't
be modifying the sources all the time.
  The generated library/ies can be found in the makes directory.

  The systems using gcc and UNIX tools can install the library/ies using:

$ make install

  If it fails or you are using another compiler you can install it by hand or
use the library without installing:
  To use the library add the include directory in your includes path and the
directory where the library was created to your libraries path (or just move
the library to the lib directory). I recommend using an environment variable
pointing to the base directory where the files are located and add the rest
to it. For example:
TVISION=e:/dj/contrib/tvision
Then $(TVISION)/include is the include directory and $(TVISION)/linux or
$(TVISION)/djgpp is the library path.
  *Note* In this way you don't need to install the library and you can make
changes in the library without needing to reinstall all the time.




4. Using the library to compile your program:
--------------------------------------------

  In order to compile your program you'll have to set your compiler to search
the header files in the directory where the Turbo Vision are installed. This
is usually achieved using the -I command option. Here is an example: suppose
the prefix is /usr/local then the headers will be installed to
/usr/local/include/rhtvision So you'll have to use
-I/usr/local/include/rhtvision as command line option for the compiler.
  All TV programs includes tv.h header and no other headers. To avoid pulling
a huge header and a lot of definitions TV have a "request" system. Suppose
you want to use the TView class, then you add to your source code:

#define Uses_TView
#include <tv.h>

  Take a look at the examples and tutorial to see real word examples. The
point is that you never do things like:

#include <tv/view.h>

  This works, but could change in the future.
  If you want to make your application more portable include the standard C
headers using definitions like Uses_stdio. This will save time because the
header will be included just once (the library could need to request it from
its own headers) and will solve some portability problems. To learn more
about read the include/compatlayer.h file and take a look at the files
located in the compat directory.
  In order to link your TV application you'll have to provide the name of the
library and the path where the library is installed.
  To simplify the above mentioned task the library provides a small program
called "rhtv-config". This program can be used to know the paths from a
script or makefile. Running the program without arguments will print the
usage information. Here is an example for a POSIX shell and the gcc
compiler:

$ gcc -o tv_logo `rhtv-config --include` tv_logo.cc `rhtv-config --dir-libs` `rhtv-config --dlibs`

  It compiles one of the example found in the examples/desklogo directory.
  For those not familiar with the above syntaxis: the reverse quotes are used
to execute a program and replace the text with the output of the executed
command. For the example I used for the include directory it means that:

$ gcc -o tv_logo `rhtv-config --include`

  Is equivalent to:

$ gcc -o tv_logo -I/usr/local/include/rhtvision

  Currently it works only for systems using gcc. If you are interested in
applying this concept to other of the supported compilers consider helping me
with some information and tests.
  Note the tool supports a switch for dynamic linking and static linking.
Currently static linking really means "link statically with TV and
dynamically with the rest of libraries". This is just to allow users to
distribute programs that doesn't need downloading TV itselft. Also note that
when linking dynamically you only have to indicate rhtv library, that's
because the library was linked providing its dependencies, but when you link
statically you have to list all the dependencies, sometimes this is long. In
my Linux system it the list of dependencies is: -lstdc++ -lncurses -lm -lgpm
-lX11 -lXmu




5. Linux/UNIX dynamic libraries:
-------------------------------

  The configure/make process generates it.
  My tests shows that a small TV example (examples/dyntxt) is only 14Kb
long when linked dynamically and without debug information. But you should
know that dynamically linked executables are slower than static ones,
specially when you call a lot of small and fast library functions, that's
because the address resolution overhead.




6. Libraries needed:
-------------------

  Consult the doc/install/tools.txt file.



7. Examples:
-----------

  I included a collection of over than 13 examples I got from the net and
ported to this TV. The examples are shipped only with the sources
distribution. See the examples directory for more information.
  For targets using the gcc compiler you can run:

$ make examples

  To compile all examples. You can also make any of the examples by changing
to the example/example_name directory and running:

$ make -f name_of_the_example.mkf

  If you are using Borland tools you must use:

> make -f name_of_the_example.bmk

  Currently Microsoft's nmake is not supported. Please contact me if you know
about the MSVC command line usage and want to help with it.





8. Dialog Editor:
----------------

  In the examples directory you'll find a directory called dlgdsn. This
directory contains sources for a dialog editor. It was created by Warlei
Alves and he asked me to maintain it. Right now I'm just including it in the
package and adapted it to compile with the last release of TV. I also made it
work under DOS and not only Linux. If enough people gets interested I think
we can do a good tool from it, but as I use EasyDiag (a library to easilly
generate dialogs) I don't have much interest on fixing all the bugs in the
dialog editor.





9. Targets supported, limitations:
---------------------------------

  The library compiles and runs in FreeBSD, DOS, Linux, QNX, Solaris and
Win32 systems. Only i386 is fully tested, but the code also works for Alpha,
ARM, HPPA, Itanium, Power PC and SPARC; we need volunteers to deeply test
those platforms. The demo program compiles for all the mentioned
architectures but I think there are some endian details that must be solved,
I fixed the ones that were visible. Joel Soete reported some success using
Linux+HPPA+gcc 3.0. All the other were tested using remote machines found in
the Source Forge's compiler farm and H.P. Test Drive.
  Starting with TV 2.0.0 we use the concept of drivers. Most drivers have
documentation in the doc directory explaining their limitations and
configuration options.





10. CPU usage:
-------------

  Since v1.0.8 the TProgram::idle() member releases the CPU to the OS. If for
some reason you want to eat 100% of the CPU or you want to use a methode
different than the used by this function just set TProgram::doNotReleaseCPU
to 1 and the class won't release the CPU.
  For Linux I just do a usleep(10), that's sleep for 10 micro s. For djgpp
I call to the __dpmi_yield() function. Note that you could use usleep for
both (DOS and Linux) but in this case you should sleep for more than 18.2 ms
to let djgpp's sleep really release the CPU. I think is easier to just call
__dpmi_yield() and explicitly release the CPU, in this way you sleep the
minimal amount of time.





11. Notes about international support:
-------------------------------------

  The configuration script detects if the internationalization support is
available in your system.
  For Linux that's part of the standard C library and all is provided as
dynamic libraries so it doesn't impact the memory usage.
  For DOS that's provided by the gettext package (gtxtXXXb.zip). Versions of
gettext prior to 0.10.37 are relative small and I think the memory used by it
is OK and you should try to use it. Since 0.10.37 libintl.a recodes the
strings on the fly, for that it needs libiconv (licvXXb.zip). This library is
a huge set of conversion tables, those tables include chinese, japanese and
korean codes, it makes the tables really huge and the size of programs linked
with it increases more than 800 Kb (more than 600 Kb using UPX). In this case
you could:
a) Just use an old version of gettext, very recommendable.
b) Generate programs with international support only when really needed.
  If you choose the second option you must configure the library to use
international support. I provide a dummy international library. This library
provides the functions that gettext provides but they do nothing.
  Why? What is the purpose? By doing it if you don't have international
support at configuration time but you install it latter no recompilation is
needed. You can also configure with international support and then create
applications without it just linking with this library. Additionally you can
configure the library to use the dummy version even if gettext is installed
(--force-dummy), that's a good option if you only need to create a few
applications with international support.
  This library is compiled and installed with the libtvfintl.a name. If you
don't have international support or want to disable it to reduce the size of
a particular application you must link with this library
  If you want to disable the fake library and all the internationalization
support use the --no-intl configuration option.
  Note: Turbo Vision have its own recoding facilities so you don't need
libiconv.




12. How to submit a patch:
-------------------------

  The simplest way is running a diff between your current directory and a
copy of the library as you downloaded it. Don't forget to specify the exact
version of the distribution and the purpose of the patchs. Use the unified
mode (-u), this mode is the best for humans ;-)




13. Memory Debuggers:
--------------------

  The library supports the MSS memory debugger version 1.2.1 for DOS. The
code have special provisions to workaround some limitations in MSS. To use it
just configure the library using the --with-mss option. You'll need to link
with the mss library as explained in the mss documentation and as shown in
the examples. MSS should work for Linux but I didn't test it.
  If you know about other memory debugger and you need to introduce some
changes in the library contact me.




14. Special thanks:
------------------

They goes to the people listed in THANKS file. If I omitted you please let me
know.




15. Contact information:
-----------------------

Salvador E. Tropea (SET)
Curapaligue 2124
3 de Febrero, Caseros
(1678) Bs.As. Argentina

Phone: (+5411) 4759-0013

e-mail: salvador@inti.gov.ar, set@computer.org, set@ieee.org
http:   www.geocities.com/SiliconValley/Vista/6552/
        www.geocities.com/set-soft/ (alias of the above mentioned)



Enjoy, SET
