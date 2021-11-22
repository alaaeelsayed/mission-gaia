#include "water.h"

const unsigned char Water::gs_bitRevTable[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF};

const unsigned short Water::gs_bitRevMasks[17] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0X0100, 0X0300, 0X0700, 0X0F00, 0X1F00, 0X3F00, 0X7F00, 0XFF00};

Water::~Water()
{
    wolf::BufferManager::DestroyBuffer(m_pIndices);

    // Delete Textures
    wolf::TextureManager::DestroyTexture(m_pH0Texture);
    wolf::TextureManager::DestroyTexture(m_pH0MinuskTexture);
    wolf::TextureManager::DestroyTexture(m_pH0Tdx);
    wolf::TextureManager::DestroyTexture(m_pH0Tdy);
    wolf::TextureManager::DestroyTexture(m_pH0Tdz);
    wolf::TextureManager::DestroyTexture(m_pDx);
    wolf::TextureManager::DestroyTexture(m_pDy);
    wolf::TextureManager::DestroyTexture(m_pDz);
    wolf::TextureManager::DestroyTexture(m_pTwiddleFactors);
    wolf::TextureManager::DestroyTexture(m_pPingPong);
    wolf::TextureManager::DestroyTexture(m_pNoise00);
    wolf::TextureManager::DestroyTexture(m_pNoise01);
    wolf::TextureManager::DestroyTexture(m_pNoise02);
    wolf::TextureManager::DestroyTexture(m_pNoise03);

    wolf::ProgramManager::DestroyProgram(m_pAmplitudeProgram);
    wolf::ProgramManager::DestroyProgram(m_pAmplitudeByTimeProgram);
    wolf::ProgramManager::DestroyProgram(m_pTwiddleFactorsProgram);
    wolf::ProgramManager::DestroyProgram(m_pInversionProgram);
    wolf::ProgramManager::DestroyProgram(m_pButterflyProgram);

    delete m_pSurface;

    // Debug
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Water::Water()
{
    if (!m_bInitialized)
    {
        // Noise Textures For Gaussian Random Numbers
        m_pNoise00 = wolf::TextureManager::CreateTexture("data/textures/water/noise/noise00.png");
        m_pNoise00->SetFilterMode(wolf::Texture::FM_Nearest, wolf::Texture::FM_Nearest);
        m_pNoise01 = wolf::TextureManager::CreateTexture("data/textures/water/noise/noise01.png");
        m_pNoise01->SetFilterMode(wolf::Texture::FM_Nearest, wolf::Texture::FM_Nearest);
        m_pNoise02 = wolf::TextureManager::CreateTexture("data/textures/water/noise/noise02.png");
        m_pNoise02->SetFilterMode(wolf::Texture::FM_Nearest, wolf::Texture::FM_Nearest);
        m_pNoise03 = wolf::TextureManager::CreateTexture("data/textures/water/noise/noise03.png");
        m_pNoise03->SetFilterMode(wolf::Texture::FM_Nearest, wolf::Texture::FM_Nearest);

        // Create empty textures

        m_pH0Texture = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pH0MinuskTexture = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pH0Tdx = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pH0Tdy = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pH0Tdz = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pDx = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pDy = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);
        m_pDy->SetFilterMode(wolf::Texture::FM_TrilinearMipmap);

        m_pDz = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pTwiddleFactors = wolf::TextureManager::CreateTexture(NULL, int(log(N) / log(2)), N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        m_pPingPong = wolf::TextureManager::CreateTexture(NULL, N, N, wolf::Texture::FMT_888832F, wolf::Texture::FMT_8888);

        // Create Compute Shader Programs
        m_pAmplitudeProgram = wolf::ProgramManager::CreateProgram("data/shaders/water/fourier_amplitude.comp", GL_COMPUTE_SHADER);
        m_pAmplitudeByTimeProgram = wolf::ProgramManager::CreateProgram("data/shaders/water/fourier_amplitude_time.comp", GL_COMPUTE_SHADER);
        m_pTwiddleFactorsProgram = wolf::ProgramManager::CreateProgram("data/shaders/water/twiddle_factors.comp", GL_COMPUTE_SHADER);
        m_pButterflyProgram = wolf::ProgramManager::CreateProgram("data/shaders/water/butterfly.comp", GL_COMPUTE_SHADER);
        m_pInversionProgram = wolf::ProgramManager::CreateProgram("data/shaders/water/inversion.comp", GL_COMPUTE_SHADER);

        // Create drawing program
        m_pRenderProgram = wolf::ProgramManager::CreateProgram("data/shaders/water/water.vsh", "data/shaders/water/water.fsh");

        // Instantiate Buffers
        _generateIndices();

        // Precompute textures
        _calculateH0K();
        _generateTwiddleFactors();

        // Initialize water surface
        m_pSurface = new Plane(m_pRenderProgram, 256);

        m_bInitialized = true;
    }
}

void Water::update(float dt)
{
    if (m_bUpdate)
    {
        _calculateH0K();
        m_bUpdate = false;
    }
    _calculateH0T();
    _butterflyOperation(m_pH0Tdx, m_pDx);
    _butterflyOperation(m_pH0Tdy, m_pDy);
    _butterflyOperation(m_pH0Tdz, m_pDz);
}

void Water::render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height)
{
    // RENDER CALLS
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), m_vPosition);
    mWorld *= glm::scale(glm::mat4(1.0f), m_vScale);

    // Bind Uniforms
    m_pRenderProgram->SetTexture("u_heightMap", m_pDy);
    m_pRenderProgram->SetUniform("u_lightDir", glm::vec3(10.0f, 10.0f, 30.0f));
    m_pRenderProgram->SetUniform("u_lightColor", glm::vec3(0.4f, 0.4f, 0.4f));
    m_pRenderProgram->SetUniform("u_waterColor", m_vWaterColor);
    m_pRenderProgram->SetUniform("u_ambientLight", glm::vec3(0.5f, 0.5f, 0.5f));

    m_pSurface->render(mProj, mView, width, height);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Debug Menu");
    ImGui::SetWindowSize(ImVec2(400.0f, 400.0f), true);
    ImGui::SliderInt("L", &L, 0, 2000);
    ImGui::SliderFloat("A", &A, 0.0f, 40.0f);
    ImGui::SliderFloat("Wind.x", &W.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Wind.y", &W.y, 0.0f, 1.0f);
    ImGui::SliderFloat("Windspeed", &S, 0.0f, 100.0f);
    ImGui::SliderFloat("Suppression", &suppression, 0.0f, 5.0f);
    ImGui::ColorPicker3("Water Color", glm::value_ptr(m_vWaterColor));
    if (ImGui::Button("Update"))
    {
        m_bUpdate = true;
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Water::_calculateH0K()
{
    // Precompute H0 texture
    W = glm::normalize(W);

    m_pAmplitudeProgram->SetTexture("noise_r0", m_pNoise00);
    m_pAmplitudeProgram->SetTexture("noise_i0", m_pNoise01);
    m_pAmplitudeProgram->SetTexture("noise_r1", m_pNoise02);
    m_pAmplitudeProgram->SetTexture("noise_i1", m_pNoise03);
    m_pAmplitudeProgram->SetUniform("N", N);
    m_pAmplitudeProgram->SetUniform("L", L);
    m_pAmplitudeProgram->SetUniform("A", A);
    m_pAmplitudeProgram->SetUniform("w", W);
    m_pAmplitudeProgram->SetUniform("windspeed", S);
    m_pAmplitudeProgram->SetUniform("suppression", suppression);

    m_pAmplitudeProgram->Bind();
    m_pH0Texture->BindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    m_pH0MinuskTexture->BindImageTexture(1, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    GLuint uiNumGroups = N / 16;
    m_pAmplitudeProgram->DispatchCompute(uiNumGroups, uiNumGroups, 1);
    glFinish();
}

void Water::_calculateH0T()
{
    W = glm::normalize(W);

    // Compute Spectrum texture over time
    m_pAmplitudeByTimeProgram->SetUniform("L", L);
    m_pAmplitudeByTimeProgram->SetUniform("t", float(glfwGetTime()));
    m_pAmplitudeByTimeProgram->SetUniform("N", N);

    m_pAmplitudeByTimeProgram->Bind();
    m_pH0Tdy->BindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    m_pH0Tdx->BindImageTexture(1, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    m_pH0Tdz->BindImageTexture(2, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    m_pH0Texture->BindImageTexture(3, 0, GL_FALSE, 0, GL_READ_ONLY, wolf::Texture::FMT_888832F);
    m_pH0MinuskTexture->BindImageTexture(4, 0, GL_FALSE, 0, GL_READ_ONLY, wolf::Texture::FMT_888832F);

    GLuint uiNumGroups = N / 16;
    m_pAmplitudeByTimeProgram->DispatchCompute(uiNumGroups, uiNumGroups, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Water::_generateTwiddleFactors()
{
    // Precompute Twiddle Factors for FFT
    m_pTwiddleFactorsProgram->SetUniform("N", N);

    uint32_t log2n = int(log(N) / log(2));

    m_pIndices->BindBase(0);

    m_pTwiddleFactorsProgram->Bind();
    m_pTwiddleFactors->BindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    GLuint uiNumGroups = N / 16;
    m_pTwiddleFactorsProgram->DispatchCompute(log2n, uiNumGroups, 1);
    glFinish();
}

void Water::_butterflyOperation(wolf::Texture *axisTexture, wolf::Texture *dest)
{
    m_pTwiddleFactors->BindImageTexture(0, 0, GL_FALSE, 0, GL_READ_ONLY, wolf::Texture::FMT_888832F);
    axisTexture->BindImageTexture(1, 0, GL_FALSE, 0, GL_READ_WRITE, wolf::Texture::FMT_888832F);
    m_pPingPong->BindImageTexture(2, 0, GL_FALSE, 0, GL_READ_WRITE, wolf::Texture::FMT_888832F);

    int log2n = int(log(N) / log(2));

    int pingpong = 0;
    for (int i = 0; i < log2n; i++)
    {
        // Horizontal Butterfly Operation
        m_pButterflyProgram->SetUniform("stage", i);
        m_pButterflyProgram->SetUniform("pingpong", pingpong);
        m_pButterflyProgram->SetUniform("direction", 0);

        m_pButterflyProgram->Bind();

        GLuint uiNumGroups = N / 16;
        m_pButterflyProgram->DispatchCompute(uiNumGroups, uiNumGroups, 1);
        glFinish();
        pingpong++;
        pingpong = pingpong % 2;
    }

    for (int i = 0; i < log2n; i++)
    {
        // Vertical Butterfly Operation
        m_pButterflyProgram->SetUniform("stage", i);
        m_pButterflyProgram->SetUniform("pingpong", pingpong);
        m_pButterflyProgram->SetUniform("direction", 1);

        m_pButterflyProgram->Bind();

        GLuint uiNumGroups = N / 16;
        m_pButterflyProgram->DispatchCompute(uiNumGroups, uiNumGroups, 1);
        glFinish();
        pingpong++;
        pingpong = pingpong % 2;
    }

    // Generate Displacement Map
    _inversionOperation(axisTexture, dest, pingpong);
}

void Water::_inversionOperation(wolf::Texture *axisTexture, wolf::Texture *dest, int pingpong)
{
    // Generate Displacement Map
    m_pInversionProgram->SetUniform("pingpong", pingpong);
    m_pInversionProgram->SetUniform("N", N);

    m_pInversionProgram->Bind();
    dest->BindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, wolf::Texture::FMT_888832F);
    axisTexture->BindImageTexture(1, 0, GL_FALSE, 0, GL_READ_ONLY, wolf::Texture::FMT_888832F);
    m_pPingPong->BindImageTexture(2, 0, GL_FALSE, 0, GL_READ_ONLY, wolf::Texture::FMT_888832F);

    GLuint uiNumGroups = N / 16;
    m_pInversionProgram->DispatchCompute(uiNumGroups, uiNumGroups, 1);
    glFinish();
}

void Water::_generateIndices()
{
    std::vector<unsigned int> indices;
    indices.resize(N);

    unsigned int num_bits = int(log(N) / log(2));

    for (int i = 0; i < N; i++)
        indices[i] = _reverseBits(i, num_bits);

    m_pIndices = wolf::BufferManager::CreateShaderStorageBuffer(indices.data(), sizeof(unsigned int) * N);
}

unsigned long Water::_reverseBits(unsigned long word, unsigned char maxLength)
{
    if (maxLength <= 8)
    {
        word = gs_bitRevTable[word << (8 - maxLength)];
    }
    else
    {
        unsigned char sc = maxLength - 8; // shift count in bits and index into masks array

        if (maxLength <= 16)
        {
            word = (gs_bitRevTable[word & 0X00FF] << sc) | gs_bitRevTable[word >> sc];
        }
        else if (maxLength & 1) // if maxLength is 17, 19, 21, or 23
        {
            word = (gs_bitRevTable[word & 0X00FF] << sc) | gs_bitRevTable[word >> sc] | (gs_bitRevTable[(word & gs_bitRevMasks[sc]) >> (sc - 8)] << 8);
        }
        else // if maxlength is 18, 20, 22, or 24
        {
            word = (gs_bitRevTable[word & 0X00FF] << sc) | gs_bitRevTable[word >> sc] | (gs_bitRevTable[(word & gs_bitRevMasks[sc]) >> (sc >> 1)] << (sc >> 1));
        }
    }

    return word;
}
