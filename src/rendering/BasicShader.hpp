#pragma once
#include "../main.hpp"

#include "IShader.hpp"


class BasicShader : public IShader {
public:
	// PUBLIC METHODS
	BasicShader();
	~BasicShader();

	void updateUniforms(const Tmatricies& _matricies, const Material& _material) const;
	//TODO: remove this with rendering engine made with threading in mind

private:
	// PRIVATE MEMBERS
	
	// PRIVATE METHODS
};
