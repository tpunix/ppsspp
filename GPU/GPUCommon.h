#pragma once

#include "GPUInterface.h"

class GPUCommon : public GPUInterface
{
public:
	GPUCommon() :
		interruptRunning(false),
		interruptEnabled(true),
		drawSyncWait(false),
		dumpNextFrame_(false),
		dumpThisFrame_(false)
	{
		for(int i = 0; i < DisplayListMaxCount; ++i)
		{
			dls[i].queued = false;
			dls[i].status = PSP_GE_LIST_NONE;
		}
	}

	virtual void EnableInterrupts(bool enable)
	{
		interruptEnabled = enable;
	}

	virtual void PreExecuteOp(u32 op, u32 diff);
	virtual void ExecuteOp(u32 op, u32 diff);
	virtual void ProcessDLQueue();
	virtual u32  UpdateStall(int listid, u32 newstall);
	virtual u32  EnqueueList(u32 listpc, u32 stall, int subIntrBase, bool head);
	virtual u32  DequeueList(int listid);
	virtual int  ListSync(int listid, int mode);
	virtual u32  DrawSync(int mode);
	virtual void DoState(PointerWrap &p);
	virtual u32  Continue();
	virtual u32  Break(int mode);

protected:
	typedef std::list<int> DisplayListQueue;

	DisplayList dls[DisplayListMaxCount];
	DisplayListQueue dlQueue;

	bool interruptRunning;
	bool interruptEnabled;

	bool drawSyncWait;

	bool dumpNextFrame_;
	bool dumpThisFrame_;

	void CheckDrawSync();

public:
	virtual DisplayList* getList(int listid)
	{
		return &dls[listid];
	}

	DisplayList* currentList()
	{
		if(dlQueue.empty())
			return NULL;
		return &dls[dlQueue.front()];
	}
};