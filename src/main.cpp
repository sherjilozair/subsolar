#include <iostream>
#include <SDL.h>

#include <sokol_gfx.h>

int main() {
    std::cout << "Hello, World!" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *window = SDL_CreateWindow("Subsolar",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 900, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    //sg_desc desc = {};
    sg_setup(&(sg_desc){0});

    const float vertices[] = {
            // positions            // colors
            0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
    };

    sg_buffer_desc vbuf_desc = {.size = sizeof(vertices), .content = vertices};
    sg_buffer vbuf = sg_make_buffer(&vbuf_desc);
    sg_bindings bind = {
            .vertex_buffers[0] = vbuf
    };

    sg_shader_desc shader_desc = {
        /* vertex attribute lookup by name is optional in GL3.x, we
           could also use "layout(location=N)" in the shader
        */
        .attrs = {
                [0].name = "position",
                [1].name = "color0"
        },
        .vs.source =
        "#version 330\n"
        "in vec4 position;\n"
        "in vec4 color0;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "  gl_Position = position;\n"
        "  color = color0;\n"
        "}\n",
        .fs.source =
        "#version 330\n"
        "in vec4 color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "  frag_color = color;\n"
        "}\n"
    };
    sg_shader shd = sg_make_shader(&shader_desc);

    sg_pipeline_desc pipeline_desc = {
        .shader = shd,
        /* if the vertex layout doesn't have gaps, don't need to provide strides and offsets */
        .layout = {
            .attrs = {
                [0].format=SG_VERTEXFORMAT_FLOAT3,
                [1].format=SG_VERTEXFORMAT_FLOAT4
            }
        }
    };

    sg_pipeline pip = sg_make_pipeline(&pipeline_desc);

    sg_pass_action pass_action = {0};

    Uint32 t1 = SDL_GetTicks();
    while (!SDL_QuitRequested()){
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        sg_begin_default_pass(&pass_action, width, height);
        sg_apply_pipeline(pip);
        sg_apply_bindings(&bind);
        sg_draw(0, 3, 1);
        sg_end_pass();
        sg_commit();

        SDL_GL_SwapWindow(window);

        Uint32 t2 = SDL_GetTicks();
        Uint32 delta = t2 - t1;
        t1 = t2;
    }
    sg_shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}