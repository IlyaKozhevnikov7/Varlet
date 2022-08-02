#pragma once

class Panel
{
public:

	Panel() = default;

	virtual ~Panel() = default;

	virtual void Init();

	virtual void Update() = 0;
};

