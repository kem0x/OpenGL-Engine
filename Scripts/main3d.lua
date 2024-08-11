VertexArray = VertexArrayObject.new();
VertexArray:Bind();

VertexBuffer = VertexBufferObject.new();
VertexBuffer:Bind();

ElementBuffer = ElementBufferObject.new();
ElementBuffer:Bind();

VertexShader = Shader.new(35633);
VertexShader:Source("E:\\source\\repos\\TheCube\\Shaders\\shader3d.vert");
VertexShader:Compile();

FragmentShader = Shader.new(35632);
FragmentShader:Source("E:\\source\\repos\\TheCube\\Shaders\\shader3d.frag");
FragmentShader:Compile();

ShaderProg = ShaderProgram.Create();
ShaderProg:Attach(VertexShader);
ShaderProg:Attach(FragmentShader);
ShaderProg:Link();

ShaderProg:VertexLocations();

ShaderProg:Use();

DefaultCamera = DebugCamera.new();

DefaultCamera:SetupInput();

SphereModel = Model.Create("E:\\source\\repos\\TheCube\\Models\\sphere\\untitled.obj",
    Location.FromWorldCoords(100, 100, 100));
SphereModel:Push();

MakeObjectHitscannable(SphereModel);

GrassTexture = Texture2D.Create("E:\\source\\repos\\TheCube\\Textures\\grass.png", 6408);
-- BrickTexture = Texture2D.Create("E:\\source\\repos\\TheCube\\Textures\\brick.png", 6408);

GrassBlock = Cube.Create(Location.FromWorldCoords(200, 200, 200), GrassTexture);
GrassBlock:Push();

Render = function()
    ShaderProg:Use();

    DefaultCamera:Update();

    ShaderProg:UniformMat4("projection", DefaultCamera.Projection);
    ShaderProg:UniformMat4("view", DefaultCamera.View);

    VertexArray:Bind();

    Drawlist.Draw(ShaderProg, VertexBuffer, ElementBuffer);
end

DebugUI = function()

    ImGuiText("-- [main3d] --");

    ImGuiText(string.format("Pitch: %f, Roll: %f, Yaw: %f",
    DefaultCamera.CameraFront.x / 0.01,
    DefaultCamera.CameraFront.y / 0.01,
    DefaultCamera.CameraFront.z / 0.01));

end