// Terrain Viewer
//
// Profiler, file: Profiler.cpp
//
// - Implementation of the Profiler class.
//
// Copyright (c) César González Segura 2015, ETSE-UV
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library.
//


#include "Profiler.h"

Profiler::Profiler()
{
	_data.drawCount = 0;
	_data.mainLoopCount = 0;
	_data.renderCount = 0;
	_data.textureCount = 0;
	_data.tileCount = 0;
	_data.uploadCount = 0;
	_data.drawCount = 0;
	_data.gpuMemoryUsed = 0;
	_data.estimatedMemoryUsed = 0;

	_data.meanDrawCommandCommitTime = 0.0f;
	_data.meanMainLoopTime = 0.0f;
	_data.meanRenderTime = 0.0f;
	_data.meanTextureLoadingTime = 0.0f;
	_data.meanTextureUploadTime = 0.0f;
	_data.meanTileRenderTime = 0.0f;
	_data.projectLoadingTime = 0.0f;
	_data.executionTime = "";

	_data.projectName = "";
}

Profiler::~Profiler()
{
}

void Profiler::SetGpuMemoryUsed(int m)
{
	if (_data.gpuMemoryUsed == 0)
	{
		_data.gpuMemoryUsed = m;
	}
}

void Profiler::SetEstimatedMemoryUsed(int m)
{
	if (_data.estimatedMemoryUsed == 0)
	{
		_data.estimatedMemoryUsed = m;
	}
}

void Profiler::SetProjectName(const std::string & name)
{
	if (_data.projectName == "")
	{
		_data.projectName = name;
	}
}

void Profiler::AddMainLoopTime(uint64_t t)
{
	_data.meanMainLoopTime += t;
	++_data.mainLoopCount;
}

void Profiler::AddRenderTime(uint64_t t)
{
	_data.meanRenderTime += t;
	++_data.renderCount;
}

void Profiler::AddTileRenderTime(uint64_t t)
{
	_data.meanTileRenderTime += t;
	++_data.tileCount;
}

void Profiler::AddDrawCommandTime(uint64_t t)
{
	_data.meanDrawCommandCommitTime += t;
	++_data.drawCount;
}
	
void Profiler::AddTextureLoadTime(uint64_t t)
{
	_data.meanTextureLoadingTime += t;
	++_data.textureCount;
}

void Profiler::AddTextureUploadTime(uint64_t t)
{
	_data.meanTextureUploadTime += t;
	++_data.uploadCount;
}
	
void Profiler::SetProjectLoadingTime(float t)
{
	_data.projectLoadingTime = t;
}

const ProfilingData & Profiler::GetProfilingData() const
{
	return _data;
}

void Profiler::SetExecutionTime(const std::string & t)
{
	_data.executionTime = t;
}

void Profiler::WriteStatistics(const std::string & path, const std::string & date, const std::string & hostInfo) const
{
	std::ofstream f(path.c_str());

	if (!f.fail())
	{
		f << "-----------------------------------------" << std::endl;
		f << "--        Execution Statistics         --" << std::endl;
		f << "-----------------------------------------" << std::endl;
		f << std::endl;
		f << "Date: " << date << std::endl;
		f << "Host information: " << hostInfo << std::endl;
		f << "Project name: " << _data.projectName << std::endl;
		f << "Execution time: " << _data.executionTime << std::endl;
		f << std::endl;

		f << "-----------------------------------------" << std::endl;
		f << "--         Loading Time Stats          --" << std::endl;
		f << "-----------------------------------------" << std::endl;
		f << std::endl;
		f << "Mean texture loading time (from storage media to RAM): " <<
			boost::lexical_cast<std::string>(_data.meanTextureLoadingTime / _data.textureCount) << " ms" << std::endl;
		f << "Mean texture upload time (from RAM to GPU memory): " <<
			boost::lexical_cast<std::string>(_data.meanTextureUploadTime / _data.uploadCount) << " ms" << std::endl;
		f << "System reported GPU memory used: " <<
			boost::lexical_cast<std::string>(_data.gpuMemoryUsed) << " MB" << std::endl;
		f << "Estimated GPU memory used: " << boost::lexical_cast<std::string>(_data.estimatedMemoryUsed) <<
			" MB" << std::endl;
		f << "- Calculated using size and bpp of heightmap and texture data)" << std::endl;
		f << "Complete project loading time: " <<
			boost::lexical_cast<std::string>(_data.projectLoadingTime) << " sec." << std::endl;
		f << std::endl;

		f << "-----------------------------------------" << std::endl;
		f << "--        Execution Time Stats         --" << std::endl;
		f << "-----------------------------------------" << std::endl;
		f << std::endl;
		f << "Mean main loop execution time: " <<
			boost::lexical_cast<std::string>(_data.meanMainLoopTime / _data.mainLoopCount) << " ms" << std::endl;
		f << "Mean project render time: " <<
			boost::lexical_cast<std::string>(_data.meanRenderTime / _data.renderCount) << " ms" << std::endl;
		f << "Mean single tile render time: " <<
			boost::lexical_cast<std::string>(_data.meanTileRenderTime / _data.tileCount) << " ms" << std::endl;
		f << "Mean draw call commit time: " <<
			boost::lexical_cast<std::string>(_data.meanDrawCommandCommitTime / _data.drawCount) << " ms" << std::endl;
		f << std::endl;

		f.flush();
		f.close();
	}
}