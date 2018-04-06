/***********************************************************
 *	@file		PullDown.h
 *	@brief		PullDown 実装
 *	@date		2018/01/29
 *	@name		hatakeyama
 **********************************************************/

#ifndef __PULLDOWN__
#define __PULLDOWN__


//preprocessor--------------------------------------------
#include "Sprite.h"
#include <vector>
#include <functional>




class PullDown : public Node
{
//variable----------------------------------------------
private:
	std::vector<Sprite*> m_sprites;

	bool m_isOpen;
	bool m_isAnimation;

	int m_currentIndex;

	int  m_openButtonSize;

	LPCSTR m_mainSpriteName;

//function-----------------------------------------------
public:

	PullDown();

	~PullDown();

	/************************************************
	 *	@brief				PullDownのインスタンス生成
	 *	@param[sprites]		初期に追加する画像
	 ************************************************/
	static PullDown* Create(Sprite* sprites);



	/********************************************
	 *	@brief				PullDownリストに追加
	 *	@param[sprite]		追加する画像
	 ********************************************/
	void AddList(Sprite* sprite);



	/*****************************************
	 *	@brief				要素の削除
	 *	@param[index]		削除する要素の番号
	 *	@remarks			番号は0からスタート
	 *****************************************/
	void RemoveAtIndex(int index);


	/******************************************************
	 *	@brief			PullDownAnimation実装
	 *******************************************************/
	void ButtonAnimation();

	void SetItemPosition();


	/**********************************************************************************
	 *	@brief				初期化する
	 **********************************************************************************/
	void Init(Sprite* sprite);

	bool Update(float delta) override;


	/**********************************************************************************
	 *	@brief				入力処理 ※まだ未完成
	 **********************************************************************************/
	void Input();



	/**********************************************************************************
	 *	@brief					画像を切り替える
	 *	@param[mainSprite]		後から追加する画像
	 **********************************************************************************/
	void SwitchMainImage(bool isClick);
};

#endif //!__PULLDOWN__

