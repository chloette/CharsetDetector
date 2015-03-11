#include "charset_detector_common_inc.h"
#include "i_charset_verifier.h"

ICharsetVerifier::ICharsetVerifier()
{
	m_dwStatesArray		= NULL;
	m_dwCclassArray		= NULL;
	m_dwStatesLength	= 0;
	m_dwCclassLength	= 0;
	m_dwSTFactor		= 0;
}

ICharsetVerifier::~ICharsetVerifier()
{
	Destroy();
}

long	ICharsetVerifier::Destroy()
{
	if(m_dwCclassArray)
	{
		free(m_dwCclassArray);
		m_dwCclassArray = 0;
	}

	if(m_dwStatesArray)
	{
		free(m_dwStatesArray);
		m_dwStatesArray = 0;
	}

	return ERR_NONE;
}

unsigned long	ICharsetVerifier::STFactor()
{
	return m_dwSTFactor;
}

unsigned long*	ICharsetVerifier::Cclass()
{
	return m_dwCclassArray;
}

unsigned long	ICharsetVerifier::CclassLength()
{
	return m_dwCclassLength;
}

unsigned long*	ICharsetVerifier::States()
{
	return m_dwStatesArray;
}

unsigned long	ICharsetVerifier::StatesLength()
{
	return m_dwStatesLength;
}

//Actually that I really don't want to know what the damned code doing below...
unsigned char ICharsetVerifier::getNextState(ICharsetVerifier* pVerifier, char b, char s)
{
	return (unsigned char)(0xff & (pVerifier->States()[(s * pVerifier->STFactor() + (pVerifier->Cclass()[(b & 0xff) >> 3] >> ((b & 7) << 2) & 0xf) & 0xff) >> 3] >> ((s * pVerifier->STFactor() + (pVerifier->Cclass()[(b & 0xff) >> 3] >> ((b & 7) << 2) & 0xf) & 0xff & 7) << 2) & 0xf));
}
