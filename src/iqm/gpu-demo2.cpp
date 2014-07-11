#include <iostream>
#include <vector>
#include <fstream>
#include <functional>

#include <cstring>
#include <cmath>

#include <GL/glew.h>
//#include <GL/freeglut.h>

#include "util.h"
#include "geom.h"
#include "iqm.h"

template<typename T>
void fatal(T message)
{
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
}

template<typename T0, typename T1>
void fatal(T0 message1, T1 message2)
{
    std::cerr << message1 << " " << message2 << " " << std::endl;
    exit(EXIT_FAILURE);
}

extern GLuint loadtexture(const char *name, int clamp);

struct iqmdata
{
    std::vector<GLuint> textures;
    GLuint notexture = 0;
    std::vector<Matrix3x4> baseframe; 
    std::vector<Matrix3x4> inversebaseframe;
    std::vector<Matrix3x4> outframe;
    std::vector<Matrix3x4> frames;
    std::vector<uchar> buf;
    iqmheader hdr;
    GLuint ebo = 0, vbo = 0, ubo = 0;
    GLint ubosize = 0, bonematsoffset = 0;
};

iqmdata model1;

struct vertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tangent[4];
    GLfloat texcoord[2];
    GLubyte blendindex[4];
    GLubyte blendweight[4];
};

void cleanupiqm(iqmdata &model)
{
    if(model.textures.size() > 0) {
        glDeleteTextures(model.textures.size(), model.textures.data());
    }
    if(model.notexture) glDeleteTextures(1, &model.notexture);

    if(model.ebo) glDeleteBuffers(1, &model.ebo);
    if(model.vbo) glDeleteBuffers(1, &model.vbo);
    if(model.ubo) glDeleteBuffers(1, &model.ubo);
}

void cleanupiqm()
{
    //cleanupiqm(model1);
}

bool loadiqmmeshes(const std::string &filename, iqmdata &model)
{
    lilswap((uint *)&model.buf[model.hdr.ofs_vertexarrays], model.hdr.num_vertexarrays*sizeof(iqmvertexarray)/sizeof(uint));
    lilswap((uint *)&model.buf[model.hdr.ofs_triangles], model.hdr.num_triangles*sizeof(iqmtriangle)/sizeof(uint));
    lilswap((uint *)&model.buf[model.hdr.ofs_meshes], model.hdr.num_meshes*sizeof(iqmmesh)/sizeof(uint));
    lilswap((uint *)&model.buf[model.hdr.ofs_joints], model.hdr.num_joints*sizeof(iqmjoint)/sizeof(uint));

    model.outframe.resize(model.hdr.num_joints);
    model.textures.resize(model.hdr.num_meshes);

    float *inposition = NULL, *innormal = NULL, *intangent = NULL, *intexcoord = NULL;
    uchar *inblendindex = NULL, *inblendweight = NULL;
    const char *str = model.hdr.ofs_text ? (char *)&model.buf[model.hdr.ofs_text] : "";
    iqmvertexarray *vas = (iqmvertexarray *)&model.buf[model.hdr.ofs_vertexarrays];
    for(int i = 0; i < (int)model.hdr.num_vertexarrays; i++)
    {
        iqmvertexarray &va = vas[i];
        switch(va.type)
        {
        case IQM_POSITION: if(va.format != IQM_FLOAT || va.size != 3) return false; inposition = (float *)&model.buf[va.offset]; lilswap(inposition, 3*model.hdr.num_vertexes); break;
        case IQM_NORMAL: if(va.format != IQM_FLOAT || va.size != 3) return false; innormal = (float *)&model.buf[va.offset]; lilswap(innormal, 3*model.hdr.num_vertexes); break;
        case IQM_TANGENT: if(va.format != IQM_FLOAT || va.size != 4) return false; intangent = (float *)&model.buf[va.offset]; lilswap(intangent, 4*model.hdr.num_vertexes); break;
        case IQM_TEXCOORD: if(va.format != IQM_FLOAT || va.size != 2) return false; intexcoord = (float *)&model.buf[va.offset]; lilswap(intexcoord, 2*model.hdr.num_vertexes); break;
        case IQM_BLENDINDEXES: if(va.format != IQM_UBYTE || va.size != 4) return false; inblendindex = (uchar *)&model.buf[va.offset]; break;
        case IQM_BLENDWEIGHTS: if(va.format != IQM_UBYTE || va.size != 4) return false; inblendweight = (uchar *)&model.buf[va.offset]; break;
        }
    }

    iqmmesh *meshes = (iqmmesh *)&model.buf[model.hdr.ofs_meshes];
    iqmjoint *joints = (iqmjoint *)&model.buf[model.hdr.ofs_joints];

    model.baseframe.resize(model.hdr.num_joints);
    model.inversebaseframe.resize(model.hdr.num_joints);
    for(int i = 0; i < (int)model.hdr.num_joints; i++)
    {
        iqmjoint &j = joints[i];
        model.baseframe[i] = Matrix3x4(Quat(j.rotate).normalize(), Vec3(j.translate), Vec3(j.scale));
        model.inversebaseframe[i].invert(model.baseframe[i]);
        if(j.parent >= 0) 
        {
            model.baseframe[i] = model.baseframe[j.parent] * model.baseframe[i];
            model.inversebaseframe[i] *= model.inversebaseframe[j.parent];
        }
    }

    for(int i = 0; i < (int)model.hdr.num_meshes; i++)
    {
        iqmmesh &m = meshes[i];
        std::cout << filename << ": loaded mesh: " << &str[m.name] << std::endl;
        model.textures[i] = loadtexture(&str[m.material], 0);
        if(model.textures[i]){
            std::cout << filename << ": loaded material: " << &str[m.material] << std::endl;
        }
    }

    iqmtriangle *tris = (iqmtriangle *)&model.buf[model.hdr.ofs_triangles];

    if(!model.ebo) glGenBuffers(1, &model.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.hdr.num_triangles*sizeof(iqmtriangle), tris, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::vector<vertex> verts(model.hdr.num_vertexes);
    for(int i = 0; i < (int)model.hdr.num_vertexes; i++)
    {
        vertex &v = verts[i];
        if(inposition) memcpy(v.position, &inposition[i*3], sizeof(v.position));
        if(innormal) memcpy(v.normal, &innormal[i*3], sizeof(v.normal));
        if(intangent) memcpy(v.tangent, &intangent[i*4], sizeof(v.tangent));
        if(intexcoord) memcpy(v.texcoord, &intexcoord[i*2], sizeof(v.texcoord));
        if(inblendindex) memcpy(v.blendindex, &inblendindex[i*4], sizeof(v.blendindex));
        if(inblendweight) memcpy(v.blendweight, &inblendweight[i*4], sizeof(v.blendweight));
    }

    if(!model.vbo) glGenBuffers(1, &model.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
    glBufferData(GL_ARRAY_BUFFER, model.hdr.num_vertexes*sizeof(vertex), verts.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

bool loadiqmanims(const std::string &filename, iqmdata &model)
{
    if(model.hdr.num_poses != model.hdr.num_joints) return false;

    lilswap((uint *)&model.buf[model.hdr.ofs_poses], model.hdr.num_poses*sizeof(iqmpose)/sizeof(uint));
    lilswap((uint *)&model.buf[model.hdr.ofs_anims], model.hdr.num_anims*sizeof(iqmanim)/sizeof(uint));
    lilswap((ushort *)&model.buf[model.hdr.ofs_frames], model.hdr.num_frames*model.hdr.num_framechannels);

    iqmpose *poses = NULL;
    iqmanim *anims = NULL;
    
    const char *str = model.hdr.ofs_text ? (char *)&model.buf[model.hdr.ofs_text] : "";
    anims = (iqmanim *)&model.buf[model.hdr.ofs_anims];
    poses = (iqmpose *)&model.buf[model.hdr.ofs_poses];
    model.frames.resize(model.hdr.num_frames * model.hdr.num_poses);
    ushort *framedata = (ushort *)&model.buf[model.hdr.ofs_frames];

    for(int i = 0; i < (int)model.hdr.num_frames; i++)
    {
        for(int j = 0; j < (int)model.hdr.num_poses; j++)
        {
            iqmpose &p = poses[j];
            Quat rotate;
            Vec3 translate, scale;
            translate.x = p.channeloffset[0]; if(p.mask&0x01) translate.x += *framedata++ * p.channelscale[0];
            translate.y = p.channeloffset[1]; if(p.mask&0x02) translate.y += *framedata++ * p.channelscale[1];
            translate.z = p.channeloffset[2]; if(p.mask&0x04) translate.z += *framedata++ * p.channelscale[2];
            rotate.x = p.channeloffset[3]; if(p.mask&0x08) rotate.x += *framedata++ * p.channelscale[3];
            rotate.y = p.channeloffset[4]; if(p.mask&0x10) rotate.y += *framedata++ * p.channelscale[4];
            rotate.z = p.channeloffset[5]; if(p.mask&0x20) rotate.z += *framedata++ * p.channelscale[5];
            rotate.w = p.channeloffset[6]; if(p.mask&0x40) rotate.w += *framedata++ * p.channelscale[6];
            scale.x = p.channeloffset[7]; if(p.mask&0x80) scale.x += *framedata++ * p.channelscale[7];
            scale.y = p.channeloffset[8]; if(p.mask&0x100) scale.y += *framedata++ * p.channelscale[8];
            scale.z = p.channeloffset[9]; if(p.mask&0x200) scale.z += *framedata++ * p.channelscale[9];
            // Concatenate each pose with the inverse base pose to avoid doing this at animation time.
            // If the joint has a parent, then it needs to be pre-concatenated with its parent's base pose.
            // Thus it all negates at animation time like so: 
            //   (parentPose * parentInverseBasePose) * (parentBasePose * childPose * childInverseBasePose) =>
            //   parentPose * (parentInverseBasePose * parentBasePose) * childPose * childInverseBasePose =>
            //   parentPose * childPose * childInverseBasePose
            Matrix3x4 m(rotate.normalize(), translate, scale);
            if(p.parent >= 0) model.frames[i*model.hdr.num_poses + j] = model.baseframe[p.parent] * m * model.inversebaseframe[j];
            else model.frames[i*model.hdr.num_poses + j] = m * model.inversebaseframe[j];
        }
    }
 
    for(int i = 0; i < (int)model.hdr.num_anims; i++)
    {
        iqmanim &a = anims[i];
        std::cout << filename << ": loaded anim: " << &str[a.name] << std::endl;
    }
    
    return true;
}

bool loadiqm(const std::string &filename, iqmdata &model)
{
    std::ifstream file (filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return false;
    
    file.read((char *)&model.hdr, sizeof(model.hdr));
   
    if(file.gcount() != sizeof(model.hdr) || memcmp(model.hdr.magic, IQM_MAGIC, sizeof(model.hdr.magic)))    
        return false;
    
    lilswap(&model.hdr.version, (sizeof(model.hdr) - sizeof(model.hdr.magic))/sizeof(uint));
    if(model.hdr.version != IQM_VERSION)
        return false;
    if(model.hdr.filesize > (16<<20)) 
        return false; // sanity check... don't load files bigger than 16 MB
    model.buf.resize(model.hdr.filesize);

    file.seekg (sizeof(model.hdr),file.beg);
    file.read((char *)(model.buf.data() + sizeof(model.hdr)), model.hdr.filesize - sizeof(model.hdr));
    if(file.gcount() != model.hdr.filesize - sizeof(model.hdr))
        return false;
    
    if(model.hdr.num_meshes > 0 && !loadiqmmeshes(filename, model)) return false;;
    if(model.hdr.num_anims > 0 && !loadiqmanims(filename, model)) return false;;
 
    return true;
}

// Note that this animates all attributes (position, normal, tangent, bitangent)
// for expository purposes, even though this demo does not use all of them for rendering.
void animateiqm(float curframe, iqmdata &model)
{
    if(model.hdr.num_frames <= 0) return;

    int frame1 = (int)floor(curframe),
        frame2 = frame1 + 1;
    float frameoffset = curframe - frame1;
    frame1 %= model.hdr.num_frames;
    frame2 %= model.hdr.num_frames;
    Matrix3x4 *mat1 = &model.frames[frame1 * model.hdr.num_joints],
              *mat2 = &model.frames[frame2 * model.hdr.num_joints];
    // Interpolate matrixes between the two closest frames and concatenate with parent matrix if necessary.
    // Concatenate the result with the inverse of the base pose.
    // You would normally do animation blending and inter-frame blending here in a 3D engine.
    iqmjoint *joints = (iqmjoint *)&model.buf[model.hdr.ofs_joints];
    for(int i = 0; i < model.hdr.num_joints; i++)
    {
        Matrix3x4 mat = mat1[i]*(1 - frameoffset) + mat2[i]*frameoffset;
        if(joints[i].parent >= 0) model.outframe[i] = model.outframe[joints[i].parent] * mat;
        else model.outframe[i] = mat;
    }
}

struct binding
{
    const char *name;
    GLint index;
};

struct shader
{
    const std::string name, vsstr, psstr;
    std::vector<binding> attribs, texs;
    GLuint vs, ps, program, vsobj, psobj;

    shader(const std::string name, const std::string vsstr, const std::string psstr, const std::vector<binding> attribs = {}, const std::vector<binding> texs = {})
        : name(name), vsstr(vsstr), psstr(psstr), attribs(attribs), texs(texs), vs(0), ps(0), program(0), vsobj(0), psobj(0)
        {}

    static void showinfo(GLuint obj, const std::string tname, const std::string name)
    {
        GLint length = 0;
        if(!tname.compare("PROG")) glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        else glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        if(length > 1)
        {
            std::vector<GLchar> log(length);
            if(!tname.compare("PROG")) glGetProgramInfoLog(obj, length, &length, log.data());
            else glGetShaderInfoLog(obj, length, &length, log.data());

            std::cout << "GLSL ERROR (" << tname << ":" << name << ")" << std::endl;
            std::cout << std::string(log.begin(),log.end()) << std::endl;                 
        }
    }

    static void compile(GLenum type, GLuint &obj, const std::string def, const std::string tname, const std::string name, bool msg = true)
    {
        const GLchar *source = (const GLchar*)(def.data() + def.find_first_not_of(" \t\r\n"));
        obj = glCreateShader(type);
        glShaderSource(obj, 1, &source, NULL);
        glCompileShader(obj);
        GLint success;
        glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            if(msg) showinfo(obj, tname, name);
            glDeleteShader(obj);
            obj = 0;
            fatal("error compiling shader");
        }
    }

    void link(const std::vector<binding> attribs = {}, bool msg = true)
    {
        program = vsobj && psobj ? glCreateProgram() : 0;
        GLint success = 0;
        if(program)
        {
            glAttachShader(program, vsobj);
            glAttachShader(program, psobj);

            for(auto a : attribs){
                glBindAttribLocation(program, a.index, a.name);
            }
            
            glLinkProgram(program);
            glGetProgramiv(program, GL_LINK_STATUS, &success);
        }
        if(!success)
        {
            if(program)
            {
                if(msg) showinfo(program, "PROG", name);
                glDeleteProgram(program);
                program = 0;
            }
            fatal("error linking shader");
        }
    }

    void compile(const std::string vsdef, const std::string psdef, const std::vector<binding> attribs = {})
    {
        compile(GL_VERTEX_SHADER,   vsobj, vsdef, "VS", name);
        compile(GL_FRAGMENT_SHADER, psobj, psdef, "PS", name);
        link(attribs, true);
    }

    void compile()
    {
        if(!vsstr.empty() && !psstr.empty()) compile(vsstr, psstr, attribs);
    }

    void set()
    {
        glUseProgram(program);
        bindtexs();
    }

    GLint getparam(const std::string pname)
    {
        return glGetUniformLocation(program, pname.data());
    }

    void bindtex(const std::string tname, GLint index)
    {
        GLint loc = getparam(tname);
        if(loc != -1) glUniform1i(loc, index);
    }

    void bindtexs()
    {
        for(auto t : texs){
            bindtex(t.name, t.index);
        }
    }
};

std::vector<binding> gpuskinattribs = { { "vtangent", 1 }, { "vweights", 6 }, { "vbones", 7 } };
std::vector<binding> gpuskintexs = { { "tex", 0 } };
shader gpuskin("gpu skin",

"#version 120\n"
"#ifdef GL_ARB_uniform_buffer_object\n"
"  #extension GL_ARB_uniform_buffer_object : enable\n"
"  layout(std140) uniform animdata\n"
"  {\n"
"     uniform mat3x4 bonemats[80];\n"
"  };\n"
"#else\n"
"  uniform mat3x4 bonemats[80];\n"
"#endif\n"
"attribute vec4 vweights;\n"
"attribute vec4 vbones;\n"
"attribute vec4 vtangent;\n"
"void main(void)\n"
"{\n"
"   mat3x4 m = bonemats[int(vbones.x)] * vweights.x;\n"
"   m += bonemats[int(vbones.y)] * vweights.y;\n"
"   m += bonemats[int(vbones.z)] * vweights.z;\n"
"   m += bonemats[int(vbones.w)] * vweights.w;\n"
"   vec4 mpos = vec4(gl_Vertex * m, gl_Vertex.w);\n"
"   gl_Position = gl_ModelViewProjectionMatrix * mpos;\n"
"   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"   mat3 madjtrans = mat3(cross(m[1].xyz, m[2].xyz), cross(m[2].xyz, m[0].xyz), cross(m[0].xyz, m[1].xyz));\n"
"   vec3 mnormal = gl_Normal * madjtrans;\n"
"   vec3 mtangent = vtangent.xyz * madjtrans; // tangent not used, just here as an example\n"
"   vec3 mbitangent = cross(mnormal, mtangent) * vtangent.w; // bitangent not used, just here as an example\n"
"   gl_FrontColor = gl_Color * (clamp(dot(normalize(gl_NormalMatrix * mnormal), gl_LightSource[0].position.xyz), 0.0, 1.0) * gl_LightSource[0].diffuse + gl_LightSource[0].ambient);\n"
"}\n",

"uniform sampler2D tex;\n"
"void main(void)\n"
"{\n"
"   gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);\n"
"}\n",

gpuskinattribs, gpuskintexs);

std::vector<binding> noskinattribs = { { "vtangent", 1 } };
std::vector<binding> noskintexs = { { "tex", 0 } };

shader noskin("no skin",

"attribute vec4 vtangent;\n"
"void main(void)\n"
"{\n"
"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"   vec3 vbitangent = cross(gl_Normal, vtangent.xyz) * vtangent.w; // bitangent not used, just here as an example\n"
"   gl_FrontColor = gl_Color * (clamp(dot(normalize(gl_NormalMatrix * gl_Normal), gl_LightSource[0].position.xyz), 0.0, 1.0) * gl_LightSource[0].diffuse + gl_LightSource[0].ambient);\n"
"}\n",

"uniform sampler2D tex;\n"
"void main(void)\n"
"{\n"
"   gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);\n"
"}\n",

noskinattribs, noskintexs);

float scale = 1, rotate = 0;

void renderiqm(iqmdata &model)
{
    static const GLfloat zero[4] = { 0, 0, 0, 0 }, 
                         one[4] = { 1, 1, 1, 1 },
                         ambientcol[4] = { 0.5f, 0.5f, 0.5f, 1 }, 
                         diffusecol[4] = { 0.5f, 0.5f, 0.5f, 1 },
                         lightdir[4] = { cosf(radians(-60)), 0, sinf(radians(-60)), 0 };

    glPushMatrix();
    glRotatef(rotate, 0, 0, -1);
    glScalef(scale, scale, scale);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, zero);
    glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, one);
    glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientcol);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusecol);
    glLightfv(GL_LIGHT0, GL_POSITION, lightdir);

    glColor3f(1, 1, 1);

    if(model.hdr.num_frames > 0)
    {
        gpuskin.set();
    
        glBindBuffer(GL_UNIFORM_BUFFER, model.ubo);
        glBufferData(GL_UNIFORM_BUFFER, model.ubosize, NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, model.bonematsoffset, model.hdr.num_joints*sizeof(Matrix3x4), model.outframe[0].a.v);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, model.ubo);
    }
    else 
    {
        noskin.set();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);

    vertex *vert = NULL;
    glVertexPointer(3, GL_FLOAT, sizeof(vertex), &vert->position);
    glNormalPointer(GL_FLOAT, sizeof(vertex), &vert->normal);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &vert->texcoord);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), &vert->tangent);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableVertexAttribArray(1);
    if(model.hdr.num_frames > 0)
    {
        glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), &vert->blendweight);
        glVertexAttribPointer(7, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex), &vert->blendindex);
        glEnableVertexAttribArray(6);
        glEnableVertexAttribArray(7);
    }

    iqmmesh *meshes = (iqmmesh *)&model.buf[model.hdr.ofs_meshes];

    for(int i = 0; i < model.hdr.num_meshes; i++)
    {
        iqmmesh &m = meshes[i];
        glBindTexture(GL_TEXTURE_2D, model.textures[i] ? model.textures[i] : model.notexture);
        glDrawElements(GL_TRIANGLES, 3*m.num_triangles, GL_UNSIGNED_INT, (iqmtriangle *)(sizeof(iqmtriangle) * m.first_triangle));
    }
   
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableVertexAttribArray(1);
    if(model.hdr.num_frames > 0)
    {
        glDisableVertexAttribArray(6);
        glDisableVertexAttribArray(7);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
}

void initgl()
{
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glDisable(GL_FOG);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    gpuskin.compile();
    
    GLuint blockidx = glGetUniformBlockIndex(gpuskin.program, "animdata"), bonematsidx;
    const GLchar *bonematsname = "bonemats";
    glGetUniformIndices(gpuskin.program, 1, &bonematsname, &bonematsidx);
    glGetActiveUniformBlockiv(gpuskin.program, blockidx, GL_UNIFORM_BLOCK_DATA_SIZE, &model1.ubosize);
    glGetActiveUniformsiv(gpuskin.program, 1, &bonematsidx, GL_UNIFORM_OFFSET, &model1.bonematsoffset);
    glUniformBlockBinding(gpuskin.program, blockidx, 0);
    if(!model1.ubo) glGenBuffers(1, &model1.ubo);

    noskin.compile();

    model1.notexture = loadtexture("notexture.tga", 0);
}

int scrw = 0, scrh = 0;

void reshapefunc(int w, int h)
{
    scrw = w;
    scrh = h;
    glViewport(0, 0, w, h);
}

float camyaw = -90, campitch = 0, camroll = 0;
Vec3 campos(20, 0, 5);

void setupcamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = double(scrw)/scrh,
             fov = radians(90),
             fovy = 2*atan2(tan(fov/2), aspect),
             nearplane = 1e-2f, farplane = 1000,
             ydist = nearplane * tan(fovy/2), xdist = ydist * aspect;
    glFrustum(-xdist, xdist, -ydist, ydist, nearplane, farplane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(camroll, 0, 0, 1);
    glRotatef(campitch, -1, 0, 0);
    glRotatef(camyaw, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(1, -1, 1);
    glTranslatef(-campos.x, -campos.y, -campos.z);
}
 
float animate = 0;

/*void timerfunc(int val)
{
    animate += 10*val/1000.0f;
    glutPostRedisplay();
    glutTimerFunc(35, timerfunc, 35);
    }*/

/*void displayfunc()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupcamera();

    animateiqm(animate, model1);
    renderiqm(model1);

    glutSwapBuffers();
    }*/

/*void keyboardfunc(uchar c, int x, int y)
{
    switch(c)   
    {
    case 27:
        exit(EXIT_SUCCESS);
        break;
    }
    }*/

/*int main(int argc, char **argv)
{
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutCreateWindow("IQM GPU Skinning Demo");

    glewExperimental = GL_TRUE; 
    glewInit();
    
    atexit(cleanupiqm);
    
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-') switch(argv[i][1])
        {
        case 's':
            if(i + 1 < argc) scale = clamp(atof(argv[++i]), 1e-8, 1e8);
            break;
        case 'r':
            if(i + 1 < argc) rotate = atof(argv[++i]);
            break;
        }
        else if(!loadiqm(argv[i],model1)) return EXIT_FAILURE;
    }
    if(model1.buf.size() == 0 && !loadiqm("mrfixit.iqm",model1)) return EXIT_FAILURE;

    initgl();
   
    glutTimerFunc(35, timerfunc, 35);
    glutReshapeFunc(reshapefunc);
    glutDisplayFunc(displayfunc);
    glutKeyboardFunc(keyboardfunc);
    glutMainLoop();
     
    return EXIT_SUCCESS;
    }*/

