#pragma once

#ifdef DEBUG
#define GL_CALL_ERROR(line) line;CheckError();
#else
#define GL_CALL_ERROR(line) line;
#endif

void CheckError();