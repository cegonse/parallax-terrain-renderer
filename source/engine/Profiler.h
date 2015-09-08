#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>

struct ProfilingData
{
	float meanMainLoopTime;
	int mainLoopCount;

	float meanRenderTime;
	int renderCount;
	
	float meanTileRenderTime;
	int tileCount;
	
	float meanDrawCommandCommitTime;
	int drawCount;

	float projectLoadingTime;
	
	float meanTextureLoadingTime;
	int textureCount;
	
	float meanTextureUploadTime;
	int uploadCount;

	std::string projectName;

	int gpuMemoryUsed;
	int estimatedMemoryUsed;

	std::string executionTime;
};

class Profiler
{
public:
	Profiler();
	~Profiler();

	void AddMainLoopTime(uint64_t t);
	void AddRenderTime(uint64_t t);
	void AddTileRenderTime(uint64_t t);
	void AddDrawCommandTime(uint64_t t);
	
	void AddTextureLoadTime(uint64_t t);
	void AddTextureUploadTime(uint64_t t);
	void SetGpuMemoryUsed(int m);
	void SetEstimatedMemoryUsed(int m);
	
	void SetProjectLoadingTime(float t);
	void SetProjectName(const std::string & name);
	void SetExecutionTime(const std::string & t);

	const ProfilingData & GetProfilingData() const;
	void WriteStatistics(const std::string & path,
						 const std::string & date,
						 const std::string & hostInfo) const;

private:
	ProfilingData _data;
};

#endif // _PROFILER_H_