#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <CL/cl.hpp>

using namespace std;

int main(void) {
	vector<cl::Platform> platforms;
	vector<cl::Device> platformDevices, allDevices, ctxDevices;
	string device_name;
	cl_uint i;

	try {
		cl::Platform::get(&platforms);
		platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &platformDevices);

		cl::Context context(platformDevices);
		ctxDevices = context.getInfo<CL_CONTEXT_DEVICES>();

		device_name = ctxDevices[0].getInfo<CL_DEVICE_NAME>();
		cout << "Your device = "<<device_name.c_str() << endl;
	}catch(cl::Error e) {
		cout << e.what() << ": Error code " << e.err() <<endl;
	}
}