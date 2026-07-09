#include "SharedMemory.h"
#include "MotionShared.h"
#include <algorithm>
#include "Alphabet.h"

SharedMemory::~SharedMemory() {
	Close();
}
bool SharedMemory::Create()
{
	m_hMap = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(MotionShared), L"MotionMemory");
	if (!m_hMap) return false;

	m_data = (MotionShared*)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MotionShared));
	if (m_data == nullptr) return false;

	m_hEvent = CreateEvent(nullptr, FALSE, FALSE, L"MotionMemoryEvent");
	if (!m_hEvent) return false;

	m_data->command = CMD_NONE;
	m_data->alphabetCount = 0;

	return true;
}
bool SharedMemory::Open()
{
	m_hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"MotionMemory");
	if (!m_hMap) return false;

	m_data = (MotionShared*)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MotionShared));
	if (m_data == nullptr) return false;

	m_hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"MotionMemoryEvent");
	if (!m_hEvent) return false;

	return true;
}
bool SharedMemory::SendCoord(const std::vector<Alphabet>& alpha)
{
	if (m_data == nullptr)
		return false;

	if (alpha.size() > 100)
		return false;

	m_data->alphabetCount = static_cast<int>(alpha.size());

	for (int i = 0; i < m_data->alphabetCount; i++)
	{
		const auto& src = alpha[i].GetCoord();

		int count = std::min<int>(
			static_cast<int>(src.size()),
			100
		);

		m_data->alphabet[i].coordCount = count;

		std::copy_n(
			src.begin(),
			count,
			m_data->alphabet[i].coord
		);
	}

	return true;
}
void SharedMemory::SendMove()
{
	if (!m_data)
		return;

	m_data->command = CMD_MOVE;
	Trigger();
}
void SharedMemory::SendStop()
{
	if (!m_data)
		return;

	m_data->command = CMD_STOP;
	Trigger();
}
void SharedMemory::SendNone()
{
	if (!m_data)
		return;

	m_data->command = CMD_NONE;
}
void SharedMemory::SendInput()
{
	if (!m_data)
		return;

	m_data->command = CMD_INPUT;
	Trigger();
}

MotionShared* SharedMemory::GetData() const
{
	return m_data;
}
void SharedMemory::Close()
{
	if (m_data)
	{
		UnmapViewOfFile(m_data);
		m_data = nullptr;
	}

	if (m_hMap)
	{
		CloseHandle(m_hMap);
		m_hMap = nullptr;
	}

	if (m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = nullptr;
	}
}

int SharedMemory::GetArraySize() const
{
	return m_data->alphabetCount;
}

const AlphabetShared* SharedMemory::GetAlphabetCoord() const
{
	return m_data->alphabet;
}

int SharedMemory::GetCommand() const
{
	return m_data->command;
}

int SharedMemory::GetCompletedCount() const
{
	return m_data->completedCount;
}

void SharedMemory::SetCompletedCount(int i)
{
	m_data->completedCount = i;
}

void SharedMemory::Trigger()
{
	if (m_hEvent)
	{
		SetEvent(m_hEvent);
	}
}

bool SharedMemory::WaitTrigger(DWORD timeoutMs)
{
	if (!m_hEvent)
		return false;

	DWORD result = WaitForSingleObject(m_hEvent, timeoutMs);

	return result == WAIT_OBJECT_0;
}