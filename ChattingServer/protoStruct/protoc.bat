set CPP_DST_DIR=%cd%/../Lib_Logic/Protobuf
set CS_DST_DIR=%cd%/../../ChattingClient/Protobuf
set SRC_DIR=%cd%
protoc.exe -I=%SRC_DIR% --cpp_out=%CPP_DST_DIR% %SRC_DIR%/Packet.proto
rem [Deprecated] protoc.exe -I=%SRC_DIR% --cpp_out=%CPP_DST_DIR% %SRC_DIR%/Define.proto
protoc.exe -I=%SRC_DIR% --csharp_out=%CS_DST_DIR% %SRC_DIR%/Packet.proto
rem [Deprecated] protoc.exe -I=%SRC_DIR% --csharp_out=%CS_DST_DIR% %SRC_DIR%/Define.proto