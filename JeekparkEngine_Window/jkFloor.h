#pragma once
#include "..\\jeekparkEngine_SOURCE\\jkGameObject.h"


namespace jk
{

	class Floor : public GameObject
	{
	public:
		Floor();
		~Floor();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

	private:

	};
}