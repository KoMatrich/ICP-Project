#include "Highlighter.h"

Highlighter::Highlighter(QTextEdit *parent)
    : QSyntaxHighlighter(parent->document())
{
    err.setBackground(Qt::red);
    after_err.setForeground(Qt::darkGray);
    cursor_color.setBackground(Qt::yellow);

    ///note tabs in formating not working

    const QString _name = "[_A-Za-z0-9]+";
    const QString _forms = "[ \\{\\}\\[\\]\\(\\)\\!\\_\\@\\#\\$\\%\\^\\&\\+\\-\\*\\/\\=\\?\\.\\,\\'\"\\;\\:\\<\\>\\\\]";
    const QString _sentence = "("+_name+"|"+_forms+"*)+";

    Rule sentence;
    sentence.start = Regex("^"+_sentence+"$");

    Rule sentence_f;
    sentence_f.start = Regex("^([\\*]{2})"+_sentence+"\\1$");

    Rule string;
    string.start = Regex("\""+_sentence);
    string.end = Regex("\"");
    string.format.setForeground(Qt::green);


    //uml body
    Rule uml;
    uml.start = Regex("^@startuml$");
    uml.end   = Regex("^@enduml$");
    uml.format.setFontWeight(QFont::Bold);

    //title
    Rule title;
    title.start = Regex("^title "+_sentence+"$");
    title.format.setFontWeight(QFont::Bold);
    uml.parts.append(title);

    //skin
    Rule skin;
    skin.start = Regex("^skin "+_sentence+"$");
    uml.parts.append(skin);

    //namedef
    Rule namedef;
    namedef.start = Regex("^'?participant \""+_name+"\" as "+_name+"$");
    uml.parts.append(namedef);

    //activate
    Rule activate;
    activate.start = Regex("^activate "+_name+"$");
    uml.parts.append(activate);

    //deactivate
    Rule deactivate;
    deactivate.start = Regex("^deactivate "+_name+"$");
    uml.parts.append(deactivate);

    //transmision
    Rule transmision;
    transmision.start = Regex("^"+_name+" (-->|->) "+_name+": "+_sentence+"$");
    uml.parts.append(transmision);
    //class atribute
    Rule atr;
    atr.start = Regex("^(\\*{0,3})"+_sentence+"\\1$");

    //class separator
    Rule sep;
    sep.start = Regex("^(([.\\-=]{2})"+_sentence+"\\2)|([.\\-=]{4})$");
    sep.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);

    //class
    Rule clas;
    clas.start = Regex("^class "+_name+"$");
    clas.end   = Regex("^end class$");
    clas.format.setFontWeight(QFont::Bold);
    clas.parts.append(atr);
    clas.parts.append(sep);
    uml.parts.append(clas);

    //object
    Rule object;
    object.start = Regex("^object "+_name+"$");
    object.end   = Regex("^end$");
    object.format.setFontWeight(QFont::Bold);
    object.parts.append(atr);
    object.parts.append(sep);
    object.parts.append(sentence_f);
    uml.parts.append(object);

    //block_note
    Rule b_note;
    b_note.start = Regex("^note (left|right)$");
    b_note.end   = Regex("^end note$");
    b_note.format.setFontWeight(QFont::Bold);
    b_note.format.setForeground(Qt::darkGreen);

    Rule note_body;
    note_body.format.setForeground(Qt::green);
    b_note.parts.append(note_body);
    uml.parts.append(b_note);

    //line note
    Rule l_note;
    l_note.start = Regex("^note (left|right): "+_sentence+"$");
    l_note.format.setFontWeight(QFont::Bold);
    l_note.format.setForeground(Qt::darkGreen);
    uml.parts.append(l_note);

    syntax.append(uml);
}

void Highlighter::getRules(Rule &current,RuleSet &parts, Path *path){
    parts = syntax; //default body of code

    if(path->length()!=0){
        //path not empty   => in body
        int i = 0;
        while(path->length() > i){
            current = parts.at(path->at(i));
            parts   = current.parts;
            i++;
        }

        if(current.end.isEmpty()){
            //one line block without end
            path->pop_back();
            getRules(current,parts,path);
        }
    }
}

int Highlighter::match(const QString &text, int &offset, Path *path)
{
    RuleSet parts;
    Rule current;
    getRules(current,parts,path);

    //find end of current block
    QRegExp end = current.end;
    if(!end.isEmpty()){
        int match_i = end.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,end.matchedLength(),current.format);
            offset += end.matchedLength();
            return -1;
        }
    }

    //find start of sub block
    for(int i = 0; i < parts.length();i++){
        Rule part = parts.at(i);
        QRegExp start = part.start;
        int match_i = start.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,start.matchedLength(),part.format);
            offset += start.matchedLength();
            return i;
        }
    }
    //no rule was matched
    return -2;
}

void Highlighter::find(const QString &text, int line, int &offset)
{
    while(line < path_stack.length()){
        //line number bigger than actual stack index
        path_stack.pop_back();
    }

    Path *path;
    if(line == 0){
        //first line
        path = new Path();
    }else{
        //load stack from previous line
        if(path_stack.length()<line){
            error(-3,"Stack error index out of bounds");
            return;
        }
        path = new Path(path_stack.at(line-1));
    }

    int result = match(text,offset,path);

    switch(result){
    default:
        path->append(result);
        break;
    case -1:
        path->pop_back();
        break;
    case -2:
        if(path->empty()){
            //outside of uml body
            //ignore all
        }else{
            //wrong input
            setCurrentBlockState(-2);
            setFormat(offset,text.length(),err);
        }
        break;
    }
    path_stack.append(*path);
}

int state_modify(int state){
    int index = state / 2;
    int mod = state % 2;
    return index*2 + (mod+1)%2;
}

void Highlighter::highlightBlock(const QString &text)
{
    switch(previousBlockState()){
    case -3:
        //internal error
        setCurrentBlockState(-3);
        break;
    case -2:
        //error ocured in syntax last check
        setFormat(0,text.length(),after_err);
        setCurrentBlockState(-2);
        break;
    case -1:
        //start of file
        path_stack.clear();
    default:
        //normal operation

        //KoMatrich magic
        if(text.isEmpty()){
            //empty line copy previous
            int state = previousBlockState();
            setCurrentBlockState(state);
            break;
        }

        if(previousBlockState() == -1){
            //first block
            int state;
            if(currentBlockState() == -1){
                //new block
                state = 0;
            }else{
                //just edit
                int dif = currentBlockState()/2 - previousBlockState()/2;
                if(dif==0)
                    state = state_modify(currentBlockState());
                else
                    state = state_modify(previousBlockState()+2);
            }
            setCurrentBlockState(state);
        }else{
            //others
            int state;
            int dif = currentBlockState()/2 - previousBlockState()/2;
            if(dif==1){
                //just change
                state = state_modify(currentBlockState());
            }else{
                //line before was delete/removed/added
                state = state_modify(previousBlockState()+2);
            }
            setCurrentBlockState(state);
        }
        //magic end

        int line = currentBlockState()/2;

        int offset = 0;
        find(text,line,offset);
        break;
    }
}
