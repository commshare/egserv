#include "Lock.h"

class CRefObject
{
public:
	CRefObject();
	virtual ~CRefObject();

	void SetLock(CLock* lock) { m_lock = lock; }
	void AddRef();
	void ReleaseRef();
private:
	int		m_refCount;
	CLock*	m_lock;
};