#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>

using namespace std;
vector<string> fun;
vector<string> dtype;
//declaring this variable which will store the whole program
string output="";

void write(string name)
{
    fstream input_f;
    //fstream output_f;
    input_f.open(name, ios::in);
    //string hpxname = "hpx_"+name;
    //output_f.open(hpxname, ios::out);

    string header = "//hpx header files\n#include <hpx/hpx_main.hpp>\n#include <hpx/include/iostreams.hpp>\n#include <hpx/hpx.hpp>\n#include <hpx/hpx_init.hpp>\n#include <hpx/include/async.hpp>\n#include <hpx/include/lcos.hpp>\n#include <hpx/include/parallel_generate.hpp>\n#include <hpx/include/parallel_sort.hpp>\n//--------------------------------\n";
    //output_f<<header;
    //output_f.close();
    input_f.close();
    output+=header;
}

void replace_std(string name)
{
    fstream input_f;
    input_f.open(name, ios::in);

    string stdc = "std::cout";
    string stde = "std::endl";
    string hpxc = "hpx::cout";
    string hpxe = "hpx::endl";

    string ch;
    while(getline(input_f,ch)){
        //takes input as one whole line and then work on it
        //we can put the checking part here
        //cout<<ch<<endl;
        for(int i=0; i<ch.length(); i++){
            if(ch[i]=='s'&&ch[i+1]=='t'&&ch[i+2]=='d'&&ch[i+3]==':'){
                if(ch[i+5]=='c'&&ch[i+6]=='o'&&ch[i+7]=='u'&&ch[i+8]=='t'){
                    i=i+8;
                    //output_f<<hpxc;
                    output+=hpxc;
                }
                else if(ch[i+5]=='e'&&ch[i+6]=='n'&&ch[i+7]=='d'&&ch[i+8]=='l'){
                    i=i+8;
                    //output_f<<hpxe;
                    output+=hpxe;
                }
                else{
                   i=i+4;
                   //output_f<<"std::";
                   output+="std::";
                }

            }
            else{
                //output_f<<ch[i];
                output+=ch[i];
            }
        }
        //output_f<<"\n";
        output+="\n";
    }
    input_f.close();
}

void replace_func(){
    string update="";
    for(int start=0; start<output.length(); start++){
        string line="";
        int check_paran=0;
        //int paren;
        while(output[start]!='\n'){
            line+=output[start];
            if(output[start]=='('){
                check_paran++;
            }
            start++;
        }
        if(output[start]=='\n'){
            //cout<<line<<endl;
            if(check_paran>0){
                int paran=0;
                for(paran=0; paran<line.length(); paran++){
                    if(line[paran]=='(')
                        break;
                }
                string func="";
                int fun_start=0;
                for(fun_start=paran-1; line[fun_start]!=' '; fun_start--){
                    if(line[fun_start]=='\t'||line[fun_start]=='\n')
                        break;
                    func=line[fun_start]+func;
                }
                if("if"==func || "while"==func){
                    //this is where we put condition for if/while
                    int inside_paran=0;
                    for(int i=paran+1; i<line.length(); i++){
                        if(line[i]=='('){
                            inside_paran++;
                            break;
                        }
                    }
                    if(inside_paran==0){
                        update+=line;
                        update+="\n";
                    }
                    else{
                        string func_in="";
                        int p_brack_in=0;
                        for(p_brack_in=paran+1; line[p_brack_in]!='('; p_brack_in++){
                            if(line[p_brack_in]==' '){
                                continue;
                            }
                            func_in+=line[p_brack_in];
                        }
                        int present=0;
                        int ind;
                        for(ind=0; ind<fun.size(); ind++){
                            if(func_in==fun[ind]){
                                present++;
                                break;
                            }
                        }
                        if(present!=0){
                            //then it is a function call
                            string medium="";
                            int end_brack=0;
                            for(end_brack=p_brack_in+1; line[end_brack]!=')'; end_brack++){
                                medium+=line[end_brack];
                            }
                            int cha_present=0;
                            for(int m=0;m<medium.length();m++){
                                if(((medium[m]>=65 && medium[m]<=90) || (medium[m]>=97 && medium[m]<=122))){
                                    cha_present++;
                                }
                            }
                            //int same_line_para=0;
                            string last="";
                            for(int i=end_brack+1; i<line.length(); i++){
                                last+=line[i];
                            }
                            if(cha_present==0){
                                update+="hpx::future<"+dtype[ind]+"> f"+to_string(start)+" = hpx::async( "+func_in+" );\n\tif(f"+to_string(start)+".get()"+last+"\n";
                            }
                            else{
                                update+="hpx::future<"+dtype[ind]+"> f"+to_string(start)+" = hpx::async( "+func_in+", "+medium+" );\n\tif(f"+to_string(start)+".get()"+last+"\n";
                            }
                        }
                        else{
                            update+=line;
                            update+="\n";
                        }
                    }
                }
                else{
                    string data_grab="";
                    for(int i=0; i<fun_start; i++){
                        if(line[i]=='\t')
                            continue;
                        if(line[i]==' ')
                            continue;
                        data_grab+=line[i];
                    }
                    int check=0;
                    for(int i=0; i<fun.size(); i++){
                        if(dtype[i]==data_grab){
                            check++;
                            break;
                        }
                    }
                    if(check==0 && func!="main"){
                        int present=0;
                        int ind;
                        for(ind=0; ind<fun.size(); ind++){
                            if(func==fun[ind]){
                                present++;
                                break;
                            }
                        }
                        if(present!=0){
                            //then it is an async call
                            data_grab="";
                            for(int i=0; i<fun_start; i++){
                                data_grab+=line[i];
                            }
                            string medium="";
                            for(int i=paran+1; line[i]!=')'; i++){
                                medium+=line[i];
                            }
                            int cha_present=0;
                            for(int m=0;m<medium.length();m++){
                                if(((medium[m]>=65 && medium[m]<=90) || (medium[m]>=97 && medium[m]<=122))){
                                    cha_present++;
                                }
                            }
                            if(dtype[ind]=="void"){
                                //have to put 2 conditions here
                                if(cha_present==0){
                                    update+="\thpx::future<void> f"+to_string(start)+" = hpx::async( "+func+" );\n";
                                }
                                else{
                                    update+="\thpx::future<void> f"+to_string(start)+" = hpx::async( "+func+", "+medium+" );\n";
                                }
                            }
                            else{
                                if(cha_present==0){
                                    update+="\thpx::future<"+dtype[ind]+"> f"+to_string(start)+" = hpx::async( "+func+" );\n\t"+data_grab+"f"+to_string(start)+".get();\n";
                                }
                                else{
                                    update+="\thpx::future<"+dtype[ind]+"> f"+to_string(start)+" = hpx::async( "+func+", "+medium+" );\n\t"+data_grab+"f"+to_string(start)+".get();\n";
                                }
                            }
                            //cout<<func<<"\t"<<data_grab<<endl;
                        }
                        else{
                            update+=line;
                            update+="\n";
                        }

                    }
                    else{
                        update+=line;
                        update+="\n";
                    }
                }

            }
            else{
                update+=line;
                update+="\n";
            }
        }
    }
    output=update;
}

void forloop()
{   int flag = 0,cb=0,cbp=0,curlbr=0,flp=0;string update = "";
    for(int i=0;i<output.length();i++)
    {
        string line = "";int sem=0;
        while(output[i]!='\n')
        {
            line+=output[i];
            i++;
        }
        for(int h =0;h<line.length();h++)
            {
                if(line[h] == ';'){sem++;}
            }
        if(sem==2){flp=1;}
        for(int st=0;st<line.length();st++)
        {
            if(line[st] == 'f' && line[st+1] == 'o' && line[st+2] == 'r' && (line[st+3] == ' ' || line[st+3] == '(') && flp==1)
            {
                flag = 1;cout<<line<<endl;
            }
            //if for() is present
            if(flag == 1)
            {
                //getting the conditions inside
                if(line[st] == '(')
                {
                    string c="";int semi =0;
                    int x=1;
                    while(line[st+x] != ')')
                    {
                        c+=line[st+x];x++;
                    }
                    //checking if it has 2 semicolons
                    for(int j=0;j<c.length();j++)
                    {
                        if(c[j] == ';')
                            semi++;
                    }
                    //if there are 2 ; , extract start and end
                    if(semi == 2)
                    {
                        string a="",b="",a1="",a2="",b2="",full = "";
                        int j=0;
                        while(c[j] != ';'){a+=c[j];j++;}j++;       // first condition
                        while(c[j] != ';'){b+=c[j];j++;}       //second condition
                        j=0;
                        while(a[j]!='='){a1+=a[j];j++;}
                        for(int k=j+1;k<a.length();k++){a2+=a[k];}
                        //cout<<a<<endl<<b;
                        int ind=0,le=0;
                        while(b[ind]!= '<'){ind++;}ind++;
                        if(b[ind] == '='){le=1;ind++;}                  // <=
                        for(int k=ind;k<b.length();k++){b2+=b[k];}
                        if(le==0){full = "\thpx::parallel::v2::for_loop_n(hpx::parallel::execution::par,"+a2+","+b2+",[&]("+a1+")";}
                        else{full = "\thpx::parallel::v2::for_loop_n(hpx::parallel::execution::par,"+a2+","+b2+"+1,[&]("+a1+")";}
                        //cout<<full<<endl;
                        cb = st;
                        while(line[cb]!='\n')
                        {
                            if(line[cb] == '{')
                            {
                                full+='{';break;                          //curly brackets present
                            }
                            cb++;
                        }
                        line = full;
                    }
                }
                //cout<<line<<endl;
            }
        }
        if(flag == 1 && flp==1)
        {
            for(int j=0;j<line.length();j++)
                {
                    if(line[j] == '{')
                    {
                        curlbr++;
                    }
                    if(line[j] == '}')
                    {
                        curlbr--;
                        if(curlbr == 0)
                        {
                            //line[j+1]=')';line[j+2]=';';
                            line+=");";
                            cb=0;flag=0;flp=0;break;
                        }
                    }
                }
        }
        update+=line;
        update+="\n";
    }
    output = update;
}

void get_fun(string name){
    //lets give another try
    //so i identify a ( and then i look for this ) and after that i check that is \n, ' ', { comes if so then its a function mostly
    int params=0, checker=0;
    int frst_params=0;
    for(int i=0; i<output.length(); i++){
        if(output[i]=='('){
            params++;
            checker=1;
            if(params == 1)
                frst_params=i;
        }
        else if(output[i]==')'){
            params--;
        }
        int k1=0;
        if(checker==1 && params==0){
            //yes it can be a function
            if((output[i]==' '||output[i]=='{'||output[i]=='\n')&&(output[i]!=';')){
                //if(output[i+1]==' '&& output[i+2]=='\n')
                string func="";
                string data="";
                int k;
                if(output[frst_params-1]!=' '){
                    for(k=frst_params-1; output[k]!=' '; k--){
                        func=output[k]+func;
                    }
                }
                else{
                    for(k=frst_params-2; output[k]!=' ';k--){
                        func=output[k]+func;
                    }
                }
                //cout<<func<<"\t"<<(output[i])<<endl;
                int cha=0;
                for(int m=k-1; output[m]!='\n'; m--){
                    //cout<<output[m]<<endl;
                    data=output[m]+data;
                }

                int spa=0;
                for(int m=0;m<data.length();m++)
                {
                    if(((data[m]>=65 && data[m]<=90) || (data[m]>=97 && data[m]<=122)))
                    {
                        cha++;
                    }
                    if(data[m]==' '){
                        spa+=1;
                    }
                }

                if(func!="main" && cha!=0 && spa==0){
                    int data_vec=0;
                    if(fun.empty()){
                        fun.push_back(func);
                        dtype.push_back(data);
                        cout<<func<<"\t"<<data<<endl;
                    }
                    else{
                        int counter=0;
                        for(string& itr:fun){
                            //string& ele = dtype[data_vec];
                            //data_vec++;
                            if(itr==func){
                                counter++;
                            }
                        }
                        if(counter==0){
                            fun.push_back(func);
                            dtype.push_back(data);
                            cout<<func<<"\t"<<data<<endl;
                        }
                    }
                }
            }
        }
    }
}

int main(){
    string fn;
    cout<<"Enter File name with extension: ";
    cin>>fn;
    write(fn);
    replace_std(fn);
    get_fun(fn);
    replace_func();
    forloop();
    //Inputting in the file
    fstream output_f;
    string hpxname = "hpx_"+fn;
    output_f.open(hpxname, ios::app);

    //inputting inside the file
    output_f<<output;
    //closing the file
    output_f.close();

    return 0;
}
