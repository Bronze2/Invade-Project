#pragma once
class CEntity
{
private:
	static UINT g_iID;
	wstring m_strName;
	UINT m_iID;
public:

	void SetName(wstring _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

	CEntity& operator =(const CEntity& _Other) {
		m_strName = _Other.m_strName;
		return *this;
	}
	CEntity();
	CEntity(const CEntity& _Other);
	virtual ~CEntity(){}
};

