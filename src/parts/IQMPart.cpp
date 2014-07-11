#include "IQMPart.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <functional>

#include <cstring>
#include <cmath>

//#include <GL/glew.h>
//#include <GL/freeglut.h>

#include "iqm/util.h"
#include "iqm/geom.h"
#include "iqm/iqm.h"

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

struct vertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tangent[4];
    GLfloat texcoord[2];
    GLubyte blendindex[4];
    GLubyte blendweight[4];
};

bool loadiqmmeshes(const std::string &filename, IQMPart &model)
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

bool loadiqmanims(const std::string &filename, IQMPart &model)
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

bool loadiqm(const std::string &filename, IQMPart &model)
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

 
IQMPart create_iqm(std::string model_name)
{
    IQMPart model1;
    loadiqm(model_name,model1);
    return model1;
}
