VertexShader = Shader.new(35633);
VertexShader:Source("E:\\source\\repos\\TheCube\\Shaders\\shader2d.vert");
VertexShader:Compile();

FragmentShader = Shader.new(35632);
FragmentShader:Source("E:\\source\\repos\\TheCube\\Shaders\\shader2d.frag");
FragmentShader:Compile();

ShaderProg = ShaderProgram.Create();
ShaderProg:Attach(VertexShader);
ShaderProg:Attach(FragmentShader);
ShaderProg:Link();

ShaderProg:VertexLocations();

ShaderProg:Use();

GrassTexture = Texture2D.Create("E:\\source\\repos\\TheCube\\Textures\\Kemoware Banner.png", 6408);

GrassSprite = Sprite.Create(GrassTexture, Vec2.Create(100.0, 100.0), Vec2.Create(200.0, 200.0), 0.0, Vec3.Create(1.0, 1.0, 1.0));
GrassSprite:Push();

Render = function()
    ShaderProg:Use();

    ShaderProg:UniformInt("image", 0);
    ShaderProg:UniformMat4("projection", Projection2D);

    Drawlist.Draw(ShaderProg);
end