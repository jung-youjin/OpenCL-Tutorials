#pragma once

#include <fstream>
#include <vector>
#include <iostream>

#define __CL_ENABLE_EXCEPTIONS
#include "CL\cl.hpp"

std::string GetPlatformName(cl_platform_id id)
{
	size_t size = 0;
	clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, nullptr, &size);

	std::string result;
	result.resize(size);
	clGetPlatformInfo(id, CL_PLATFORM_NAME, size,
		const_cast<char*> (result.data()), nullptr);

	return result;
}

std::string GetDeviceName(cl_device_id id)
{
	size_t size = 0;
	clGetDeviceInfo(id, CL_DEVICE_NAME, 0, nullptr, &size);

	std::string result;
	result.resize(size);
	clGetDeviceInfo(id, CL_DEVICE_NAME, size,
		const_cast<char*> (result.data()), nullptr);

	return result;
}

const char *getErrorString(cl_int error)
{
	switch (error){
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}

void CheckError(cl_int error)
{
	if (error != CL_SUCCESS) {
		std::cerr << "OpenCL call failed with error " << getErrorString(error) << std::endl;
		std::exit(1);
	}
}

enum VerboseLevel
{
	VERBOSE_OFF,
	VERBOSE_BRIEF,
	VERBOSE_DETAILED,
	VERBOSE_FULL
};

void AddSources(cl::Program::Sources& sources, const std::string& file_name) {
	//TODO: add file existence check
	std::string* source_code = new std::string(std::istreambuf_iterator<char>(std::ifstream(file_name)), (std::istreambuf_iterator<char>()));
	sources.push_back(std::make_pair((*source_code).c_str(), source_code->length() + 1));
}

void ListPlatformsDevices(VerboseLevel verbose_level) {
	std::vector<cl::Platform> platforms;

	if (verbose_level != VERBOSE_OFF)
	{
		std::cout << "----------------------------------------------------------------" << std::endl;
		try
		{
			cl::Platform::get(&platforms);

			std::cout << "Found " << platforms.size() << " platform(s):" << std::endl;

			for (unsigned int i = 0; i < platforms.size(); i++)
			{
				std::cout << "\nPlatform " << i << ", " << platforms[i].getInfo<CL_PLATFORM_NAME>() << ", version: " << platforms[i].getInfo<CL_PLATFORM_VERSION>();

				if (verbose_level > VERBOSE_BRIEF)
					std::cout << ", vendor: " << platforms[i].getInfo<CL_PLATFORM_VENDOR>();

				if (verbose_level > VERBOSE_DETAILED)
					std::cout << ", profile: " << platforms[i].getInfo<CL_PLATFORM_PROFILE>() << ", extensions: " << platforms[i].getInfo<CL_PLATFORM_EXTENSIONS>();

				std::cout << std::endl;

				std::vector<cl::Device> devices;

				platforms[i].getDevices((cl_device_type)CL_DEVICE_TYPE_ALL, &devices);

				std::cout << "\n   Found " << devices.size() << " device(s):" << std::endl;

				for (unsigned int j = 0; j < devices.size(); j++)
				{
					std::cout << "\n      Device " << j << ", " << devices[j].getInfo<CL_DEVICE_NAME>() << ", version: " << devices[j].getInfo<CL_DEVICE_VERSION>();

					if (verbose_level > VERBOSE_BRIEF)
					{
						std::cout << ", vendor: " << devices[j].getInfo<CL_DEVICE_VENDOR>();
						cl_device_type device_type = devices[j].getInfo<CL_DEVICE_TYPE>();
						std::cout << ", type: ";
						if (device_type & CL_DEVICE_TYPE_DEFAULT)
							std::cout << "DEFAULT ";
						if (device_type & CL_DEVICE_TYPE_CPU)
							std::cout << "CPU ";
						if (device_type & CL_DEVICE_TYPE_GPU)
							std::cout << "GPU ";
						if (device_type & CL_DEVICE_TYPE_ACCELERATOR)
							std::cout << "ACCELERATOR ";
						std::cout << ", compute units: " << devices[j].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
					}

					if (verbose_level > VERBOSE_DETAILED)
					{
					}

					std::cout << std::endl;
				}
			}
			std::cout << "----------------------------------------------------------------" << std::endl;
		}
		catch (cl::Error err) {
			std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
		}
	}
}

cl::Context GetContext(int platform_id, int device_id) {
	std::vector<cl::Platform> platforms;

	cl::Platform::get(&platforms);

	for (unsigned int i = 0; i < platforms.size(); i++)
	{
		std::vector<cl::Device> devices;
		platforms[i].getDevices((cl_device_type)CL_DEVICE_TYPE_ALL, &devices);

		for (unsigned int j = 0; j < devices.size(); j++)
		{
			if ((i == platform_id) && (j == device_id))
				return cl::Context({ devices[j] });
		}
	}

	return cl::Context();
}
