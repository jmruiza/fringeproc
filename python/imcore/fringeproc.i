/* -*- C -*-  (not really, but good for syntax highlighting) */
%module fringeproc

%{
  #define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"
%init %{
  import_array();
%}

%include "demodgabor.i"
%include "scanner.i"
%include "unwrap.i"


