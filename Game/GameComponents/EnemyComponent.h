#pragma once
#include "Components/CharacterComponent.h"

class EnemyComponent : public cool::CharacterComponent{
public:
	CLASS_DECLARATION(EnemyComponent)

		virtual void Initialize() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(cool::Actor* other) override;
	virtual void OnCollisionExit(cool::Actor* other) override;

	virtual void OnNotify(const cool::Event& event) override;

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

protected:
};