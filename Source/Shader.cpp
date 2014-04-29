#include <iostream>
#include <fstream>//ifstream
#include <string>
#include <stdarg.h>/* va_list, va_start, va_arg, va_end */

#include "Shader.h"

std::string strWord(int index, std::string line);

Shader::Shader(const char* ccs_VertFile,const char* ccs_FragFile,const char* ccs_GeomFile){
	GLint iPass = 0;
	GLchar cErrorLog[1024];
	GLuint ShaderProgram = glCreateProgram();
	if (ShaderProgram == 0){
		printf("Error creating ShaderProgram \n '%i' \n",ShaderProgram);
		iShaderID = -1;
		return;
	}
	if (ccs_VertFile != nullptr){
		char* csvertShad = LoadShaderFile(ccs_VertFile);
		if (csvertShad != nullptr){
			Load(ShaderProgram,GL_VERTEX_SHADER,csvertShad);
		}else{
			iShaderID = -1;
			return;
		}
	}if (ccs_FragFile != nullptr){
		char* csfragShad = LoadShaderFile(ccs_FragFile);
		if (csfragShad != nullptr){
			Load(ShaderProgram,GL_FRAGMENT_SHADER,csfragShad);
		}else{
			iShaderID = -1;
			return;
		}
	}if (ccs_GeomFile != nullptr){
		char* csGeomShad = LoadShaderFile(ccs_GeomFile);
		if (csGeomShad != nullptr){
			Load(ShaderProgram,GL_GEOMETRY_SHADER,csGeomShad);
		}else{
			iShaderID = -1;
			return;
		}
	}
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram,GL_LINK_STATUS,&iPass);
	if (iPass == 0){
		glGetProgramInfoLog(ShaderProgram,sizeof(cErrorLog),NULL,cErrorLog);
		printf("Error while linking ShaderProgram \n '%s' \n",cErrorLog);
		iShaderID = -1;
		return;
	}
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram,GL_LINK_STATUS,&iPass);
	if (iPass == 0){
		glGetProgramInfoLog(ShaderProgram,sizeof(cErrorLog),NULL,cErrorLog);
		printf("Error while validating ShaderProgram \n '%s' \n",cErrorLog);
		iShaderID = -1;
		return;
	}
	iShaderID = ShaderProgram;

	//	Find all the uniform names and their locations
	if (ccs_VertFile != nullptr)
		FindUniforms(ccs_VertFile);
	if (ccs_FragFile != nullptr)
		FindUniforms(ccs_FragFile);
	if (ccs_GeomFile != nullptr)
		FindUniforms(ccs_GeomFile);
}
Shader::~Shader(){
	if (iShaderID != 0){
		glDeleteProgram(iShaderID);
	}
}
void Shader::Remove(){
	if (iShaderID != 0){
		glDeleteProgram(iShaderID);
	}
}
void Shader::Use(){
	if (iShaderID != 0){
		glUseProgram(iShaderID);
	}
}

void Shader::Load(GLuint glui_ShaderProgram,GLenum gle_ShaderType,const char* ccs_File){
	GLint iPass = 0;
	GLchar cErrorLog[1024];
	GLint iLength = strlen(ccs_File);
	GLuint ShaderObj = glCreateShader(gle_ShaderType);
	if (ShaderObj == 0){
		printf("Error with ShaderObj %d\n",gle_ShaderType);
		return;
	}
	glShaderSource(ShaderObj,1,&ccs_File,&iLength);
	glCompileShader(ShaderObj);
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &iPass);
	if (iPass == 0){
		glGetShaderInfoLog(ShaderObj,sizeof(cErrorLog),NULL,cErrorLog);
        printf("Error compiling shader type %d: '%s'\n", gle_ShaderType, cErrorLog);
		return;
	}else{
		printf("Shader File Loaded.\n");
	}
	glAttachShader(glui_ShaderProgram,ShaderObj);
}
char* Shader::LoadShaderFile(const char* ccs_FilePath){
	std::ifstream is(ccs_FilePath,std::ifstream::binary);
	if (!is){
        printf("Error Opening shader file at location : %s \n",ccs_FilePath);
		return nullptr;
	}
	is.seekg(0,is.end);
	int length = (int)is.tellg();
	is.seekg(0,is.beg);
	char* cReturn = new char[length];
	is.read(cReturn,length);
	cReturn[length] = 0;
	//printf("Loaded shader %s \n%s\n",ccs_FilePath,cReturn);
	return (cReturn);
}

void Shader::FindUniforms(int i_amount,...){
	if (iShaderID == 0)
		return;
	lUniforms.clear();
	va_list vaItems;
	va_start(vaItems,i_amount);
	for (int i = 0; i < i_amount; i++){
		char* cVaItem = va_arg(vaItems,char*);
		lUniforms.push_back( std::pair<int,char*>(glGetUniformLocation(iShaderID,cVaItem),cVaItem) );
		printf("Location : %i Found uniform with name : %s\n",lUniforms.at(i).first,cVaItem);
	}
}
void Shader::FindUniforms(const char* ccs_FilePath) {
	if (iShaderID == 0)
		return;
	//!!WARNING!! WILL ADD ONTO CURRENT PAIR, PAIR IS NOT CLEARED
	std::ifstream ifs;
	ifs.open(ccs_FilePath);
	char* search = "uniform";											//	what to search for (could be changed to attrib)
	std::string line;
	while (!ifs.eof()) {
		std::getline(ifs,line);
		int offset = line.find(search, 0);								//	how far from the start of the line uniform occurs
		if (offset != std::string::npos) {
			std::string name = strWord(3, line.substr(offset));			//	third word from uniform is the name of the uniform
			char* s = new char[name.length()+1];						//	getUniformLocation requies a char *
			std::strcpy(s,name.c_str());
			unsigned int loc = glGetUniformLocation(iShaderID,s);
			std::cout << loc << " : \"" << s << "\"\n";
			//	!!NEED TO CHECK IF ITEM IS ALREADY IN LIST!!
			lUniforms.push_back( std::pair<int,char*>(loc,s) );
		}
	}
	ifs.close();
}

void Shader::SetUniform(char* cs_Name,char* cs_Type,int count,...){
	if (iShaderID == 0)
		return;
	va_list vaItems;
	va_start(vaItems,count);
	int iLoc = -1;
	for (int i = 0; i < lUniforms.size(); i++){
		if (std::strcmp(lUniforms.at(i).second,cs_Name) == 0){
			iLoc = lUniforms.at(i).first;
			break;
		}
	}

	//Make sure we enable ourself
	Use();
	//Singles
	//Float
	if (cs_Type == "1f"){
		float data = va_arg(vaItems, double);		
		glUniform1f(iLoc,data);
	}if (cs_Type == "2f"){
		float data1 = va_arg(vaItems, double);		float data2 = va_arg(vaItems, double);		
		glUniform2f(iLoc,data1,data2);
	}if (cs_Type == "3f"){
		float data1 = va_arg(vaItems, double);		float data2 = va_arg(vaItems, double);		float data3 = va_arg(vaItems, double);
		glUniform3f(iLoc,data1,data2,data3);
	}if (cs_Type == "4f"){
		float data1 = va_arg(vaItems, double);		float data2 = va_arg(vaItems, double);		float data3 = va_arg(vaItems, double);		float data4 = va_arg(vaItems, double);
		glUniform4f(iLoc,data1,data2,data3,data4);
	}
	//Int
	if (cs_Type == "1i"){
		int data = va_arg(vaItems, int);		
		glUniform1i(iLoc,data);
	}if (cs_Type == "2i"){
		int data1 = va_arg(vaItems, int);		int data2 = va_arg(vaItems, int);		
		glUniform2i(iLoc,data1,data2);
	}if (cs_Type == "3i"){
		int data1 = va_arg(vaItems, int);		int data2 = va_arg(vaItems, int);		int data3 = va_arg(vaItems, int);
		glUniform3i(iLoc,data1,data2,data3);
	}if (cs_Type == "4i"){
		int data1 = va_arg(vaItems, int);		int data2 = va_arg(vaItems, int);		int data3 = va_arg(vaItems, int);		int data4 = va_arg(vaItems, int);
		glUniform4i(iLoc,data1,data2,data3,data4);
	}
	//Unsigned Int
	if (cs_Type == "1ui"){
		unsigned int data = va_arg(vaItems, unsigned int);		
		glUniform1ui(iLoc,data);
	}if (cs_Type == "2ui"){
		unsigned int data1 = va_arg(vaItems, unsigned int);		unsigned int data2 = va_arg(vaItems, unsigned int);		
		glUniform2ui(iLoc,data1,data2);
	}if (cs_Type == "3ui"){
		unsigned int data1 = va_arg(vaItems, unsigned int);		unsigned int data2 = va_arg(vaItems, unsigned int);		unsigned int data3 = va_arg(vaItems, unsigned int);
		glUniform3ui(iLoc,data1,data2,data3);
	}if (cs_Type == "4ui"){
		unsigned int data1 = va_arg(vaItems, unsigned int);		unsigned int data2 = va_arg(vaItems, unsigned int);		unsigned int data3 = va_arg(vaItems, unsigned int);		unsigned int data4 = va_arg(vaItems, unsigned int);
		glUniform4ui(iLoc,data1,data2,data3,data4);
	}
	//Arrays
	//float Array
	if (cs_Type == "1fv"){
		float *data = va_arg(vaItems, float*);		glUniform1fv(iLoc,count,data);
	}if (cs_Type == "2fv"){
		float *data = va_arg(vaItems, float*);		glUniform2fv(iLoc,count,data);
	}if (cs_Type == "3fv"){
		float *data = va_arg(vaItems, float*);		glUniform3fv(iLoc,count,data);
	}if (cs_Type == "4fv"){
		float *data = va_arg(vaItems, float*);		glUniform4fv(iLoc,count,data);
	}
	//Int Array
	if (cs_Type == "1iv"){
		int *data = va_arg(vaItems, int*);		glUniform1iv(iLoc,count,data);
	}if (cs_Type == "2iv"){
		int *data = va_arg(vaItems, int*);		glUniform2iv(iLoc,count,data);
	}if (cs_Type == "3iv"){
		int *data = va_arg(vaItems, int*);		glUniform3iv(iLoc,count,data);
	}if (cs_Type == "4iv"){
		int *data = va_arg(vaItems, int*);		glUniform4iv(iLoc,count,data);
	}
	//Unsigned Int Array
	if (cs_Type == "1uiv"){
		unsigned int *data = va_arg(vaItems, unsigned int*);		glUniform1uiv(iLoc,count,data);		
	}if (cs_Type == "2uiv"){
		unsigned int *data = va_arg(vaItems, unsigned int*);		glUniform2uiv(iLoc,count,data);
	}if (cs_Type == "3uiv"){
		unsigned int *data = va_arg(vaItems, unsigned int*);		glUniform3uiv(iLoc,count,data);
	}if (cs_Type == "4uiv"){
		unsigned int *data = va_arg(vaItems, unsigned int*);		glUniform4uiv(iLoc,count,data);
	}
	//Matrix
	//Same Dimension Matricies
	if (cs_Type == "m2fv"){
		bool norm = va_arg(vaItems,bool);		float *m2fv = va_arg(vaItems,float*);		glUniformMatrix2fv(iLoc,count,norm,m2fv);
	}if (cs_Type == "m2fv"){
		bool norm = va_arg(vaItems,bool);		float *m3fv = va_arg(vaItems,float*);		glUniformMatrix3fv(iLoc,count,norm,m3fv);
	}if (cs_Type == "m4fv"){
		bool norm = va_arg(vaItems,bool);		float* m4fv = va_arg(vaItems,float*);		glUniformMatrix4fv(iLoc,count,norm,m4fv);
	}
	//Odd Dimension Matricies
	if (cs_Type == "m23fv"){
		bool norm = va_arg(vaItems,bool);		float *m23fv = va_arg(vaItems,float*);		glUniformMatrix2x3fv(iLoc,count,norm,m23fv);
	}if (cs_Type == "m32fv"){
		bool norm = va_arg(vaItems,bool);		float *m32fv = va_arg(vaItems,float*);		glUniformMatrix3x2fv(iLoc,count,norm,m32fv);
	}if (cs_Type == "m34fv"){
		bool norm = va_arg(vaItems,bool);		float *m34fv = va_arg(vaItems,float*);		glUniformMatrix3x4fv(iLoc,count,norm,m34fv);
	}if (cs_Type == "m43fv"){
		bool norm = va_arg(vaItems,bool);		float *m43fv = va_arg(vaItems,float*);		glUniformMatrix4x3fv(iLoc,count,norm,m43fv);
	}if (cs_Type == "m24fv"){
		bool norm = va_arg(vaItems,bool);		float *m24fv = va_arg(vaItems,float*);		glUniformMatrix2x4fv(iLoc,count,norm,m24fv);
	}if (cs_Type == "m42fv"){
		bool norm = va_arg(vaItems,bool);		float *m42fv = va_arg(vaItems,float*);		glUniformMatrix4x2fv(iLoc,count,norm,m42fv);
	}
	return;
}
void Shader::SetAttribs(int i_Count,...){
	if (iShaderID == 0)
		return;
	//Make sure we enable ourself
	Use();
	va_list vaItems;
	va_start(vaItems,i_Count);
	for (int i = 0; i < i_Count; i++){
		int loc = va_arg(vaItems,int);
		char* name = va_arg(vaItems,char*);
		printf("Set attrib at location : %i with name : %s \n",loc,name);
		glBindAttribLocation(iShaderID,loc,name);
	}
}
	
void Shader::SetTexture(char* cs_Name,unsigned int ShaderTexID,unsigned int ModelTexID){
	GLint uDiffuseTexture = glGetUniformLocation(iShaderID, cs_Name);
	glActiveTexture( 0x84C0 + ShaderTexID );
	glBindTexture( GL_TEXTURE_2D, ModelTexID );
	glUniform1i( uDiffuseTexture, ShaderTexID );
	glActiveTexture( 0 );
}
//==================FUNCTIONS==================//
std::string strWord(int index, std::string line) {
	int count = 0;									//	How many words have been gone through
	std::string word;
	for (unsigned int i = 0; i < line.length(); ++i) {
		//	If the character is a word ender (space, comma, colon, semicolon), then we are done reading a word
		if (line[i]== ' ' || line[i]== ';' || line[i]== ',' || line[i]== '.' || line[i]== ':') {		
			if (word.length() > 0) {				//	Make sure it has gathered a word (stops cases of double space or ' : ' increasing the word count)
				count++;
				if (count == index)
					//word.push_back('\n');
					return word;
				word = "";							//	Wasn't the number word we were looking for
			}
		}
		else
			word += line[i];						//	Add on the characters to the word as it walks through
	}
	return word;
}
