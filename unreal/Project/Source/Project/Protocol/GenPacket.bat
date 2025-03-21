protoc.exe --proto_path=./ --cpp_out=./  ./Gstruct.proto
IF ERRORLEVEL 1 PAUSE