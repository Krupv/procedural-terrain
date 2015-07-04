#pragma once
#include "../main.hpp"

#include "IShader.hpp"


class BasicTerrainShader : public IShader {
public:
	// PUBLIC METHODS
	BasicTerrainShader();
	~BasicTerrainShader();

	void updateUniforms(const Tmatricies& _matricies, const Material& _material) const;
	//TODO: remove this with rendering engine made with threading in mind
	//void updateUniforms( const Transform& _worldTransformation, const Camera& _camera);

private:
	// PRIVATE MEMBERS
	
	// PRIVATE METHODS
};
