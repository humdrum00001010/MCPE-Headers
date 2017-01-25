#pragma once



class ScreenRenderer {
public:
	static void *singletonPtr;
	static void *mScreenMaterials;
	static void *guiTex;
	static void *touchGuiTex;
	static void *touchGui2Tex;
	static void *spritesheetTex;
	static void *spritesheetTex2;

	ScreenRenderer();
	/*void singleton();
	void reloadResources(mce::TextureGroup&);
	void drawRect(int, int, int, int, Color const&, int);
	void drawCenteredString(Font*, std::string const&, int, int, Color const&);
	void blit(mce::TexturePtr const&, int, int, int, int, int, int, int, int, mce::MaterialPtr*);
	void drawString(Font*, std::string const&, int, int, Color const&);
	void fill(int, int, int, int, Color const&) const;
	void fill(float, float, float, float, Color const&) const;
	void blit(mce::TexturePtr const&, float, float, int, int, float, float, int, int, mce::MaterialPtr*);
	void fillGradient(int, int, int, int, Color const&, Color const&);
	void fillHorizontalGradient(glm::detail::tvec2<float> const&, glm::detail::tvec2<float> const&, Color const&, Color const&);
	void fillGradient(glm::detail::tvec2<float> const&, glm::detail::tvec2<float> const&, Color const&, Color const&);
	void blit(mce::TexturePtr const&, std::vector<BlitParams, std::allocator<BlitParams> > const&);
	void getMaterial(ScreenRenderer::ScreenMaterial) const;
	void getMaterial(ScreenRenderer::ScreenMaterial) const;
	void ScreenRenderer();
	void fillStencil(float, float, float, float) const;*/
};