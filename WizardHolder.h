// WizardHolder.h: interface for the CWizardHolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZARDHOLDER_H__2BC128AD_1F32_481A_AE3C_694ACC1B2D97__INCLUDED_)
#define AFX_WIZARDHOLDER_H__2BC128AD_1F32_481A_AE3C_694ACC1B2D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMP3WizardDlg;
class CWizChoosePage;
class CWizGetTagPage;
class CWizFileIDPage;
class CWizFuzzyPage;
class CWizProcessPage;
class CWizProcessOptPage;
class CWizStripTagsPage;
class CWizSearchAGChoosePage;
class CWizAckDiscPage;

class CWizardHolder  
{
public:
	void ShowWizard(CWnd * pWnd);
	CWizardHolder();
	virtual ~CWizardHolder();

	CMP3WizardDlg  *m_pWizardDlg;
	CWizChoosePage *m_pChoosePage;
	CWizGetTagPage *m_pGetTagPage;	
	CWizFileIDPage *m_pFileIDPage;
	CWizFuzzyPage  *m_pFuzzyPage;
	CWizProcessPage *m_pProcessPage;
	CWizProcessOptPage *m_pProcessOptPage;
	CWizStripTagsPage *m_pStripPage;
	CWizSearchAGChoosePage *m_pSearchAGChoosePage;
	CWizAckDiscPage *m_pAckDiscPage;

protected:
	void DeleteWizard();
};

#endif // !defined(AFX_WIZARDHOLDER_H__2BC128AD_1F32_481A_AE3C_694ACC1B2D97__INCLUDED_)
