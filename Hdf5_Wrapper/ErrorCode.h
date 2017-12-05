#ifndef ERROR_CODE_H
#define ERROR_CODE_H

enum ERROR_CODE
{
	ERROR_NO_ERROR = 0,
	ERROR_PARAM_INVALID,
	ERROR_FILE_NOT_EXIST,
	ERROR_WRITE_HDF5_FAILED,
	ERROR_JUDGE_FILE_EXCEPTION,
	ERROR_FILE_FORMAT_INVALID,
	ERROR_READ_HDF5_FAILED
};
#endif