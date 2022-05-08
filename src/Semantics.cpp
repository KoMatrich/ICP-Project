#include "Semantics.h"

QString UMLClass::getClassName()
{
    return this->class_name;
}

void UMLClass::updateName(QString name)
{
    if (this->class_name == name)
        return;

    this->has_changed = true;
    this->class_name = name;
}

void UMLClass::updateType(bool is_interface)
{
    if (this->is_interface == is_interface)
        return;

    this->has_changed = true;
    this->is_interface = is_interface;
}

void UMLClass::addProperty(UMLProperty new_p, bool isMethod, size_t n)
{
    std::vector<UMLProperty>* search;
    if (isMethod)
        search = &this->methods;
    else
        search = &this->attributes;

    //updating
    if (n < (*search).size()) {
        this->has_changed |= (*search)[n].updateProperty(new_p);
    }
    //adding
    else {
        search->push_back(new_p);
        this->has_changed = true;
    }
}

void UMLClass::removeExceedingProperties(size_t a, size_t m)
{
    if (this->attributes.size() > a) {
        while (this->attributes.size() > a) {
            this->attributes.pop_back();
        }
        this->has_changed = true;
    }

    if (this->methods.size() > m) {
        while (this->methods.size() > m) {
            this->methods.pop_back();
        }
        this->has_changed = true;
    }
}

void UMLClass::removeExceedingRelations(size_t r)
{
    if (this->relations.size() > r) {
        while (this->relations.size() > r) {
            this->relations.pop_back();
        }
        this->has_changed = true;
    }
}

void UMLClass::addRelation(UMLRelation new_p, size_t n)
{
    //updating
    if (n < this->relations.size()) {
        this->has_changed |= this->relations[n].updateRelationParams(new_p);
    }
    //adding
    else {
        this->relations.push_back(new_p);
        this->has_changed = true;
    }
}

bool UMLClass::updatePosition(int pos, bool is_x, size_t i)
{
    if (is_x) {
        if (this->x_set) {
            this->setErrorFlag(true);
            CodeService::formatLine(i, HLevel::LEVEL_ERROR);
            VitaPrint("[ERROR]: Duplicate x coordinate, please fix in the code edit.");
            return false;
        }
        this->x_set = true;
        this->x = pos;
        this->x_line = i;
    } else {
        if (this->y_set) {
            this->setErrorFlag(true);
            CodeService::formatLine(i, HLevel::LEVEL_ERROR);
            VitaPrint("[ERROR]: Duplicate y coordinate, please fix in the code edit.");
            return false;
        }
        this->y_set = true;
        this->y = pos;
        this->y_line = i;
    }
    return true;
}

void UMLClass::printProperties()
{
    for (size_t i = 0; i < attributes.size(); i++) {
        VitaPrint(attributes[i].toString());
    }
    for (size_t i = 0; i < methods.size(); i++) {
        VitaPrint(methods[i].toString());
    }
}

bool UMLClass::updateInherited(bool isMethod, std::vector<UMLProperty> src)
{
    for (UMLProperty p : src) {
        if (p.getType() != QString("-")) {
            if (isMethod)
                this->inheritedMethods.push_back(p);
            else
                this->inheritedAttributes.push_back(p);
        }
    }
    return false;
}

QString UMLProperty::toString()
{
    return QString("\t" + p_mod + " " + p_type + " " + p_name);
}

bool UMLProperty::updateProperty(UMLProperty new_p)
{
    bool changed = false;
    changed |= new_p.p_mod != this->p_mod;
    changed |= new_p.p_type != this->p_type;
    changed |= new_p.p_name != this->p_name;

    if (changed) {
        this->p_mod = new_p.p_mod;
        this->p_type = new_p.p_type;
        this->p_name = new_p.p_name;
    }

    return changed;
}

QString UMLRelation::toString()
{

    return QString("in relation " + QString(RuleIDtoString(this->type)) + " with " + this->entity);
}

QString UMLRelation::getEntity()
{
    return this->entity;
}

bool UMLRelation::updateRelationParams(UMLRelation new_r)
{
    bool changed = false;
    changed |= new_r.entity != this->entity;
    changed |= new_r.type != this->type;
    changed |= new_r.pos != this->pos;

    if (changed) {
        this->entity = new_r.entity;
        this->type = new_r.type;
        this->pos = new_r.pos;
    }

    return changed;
}

bool UMLRelation::updateRelationConnectors(size_t new_id)
{
    bool changed = false;
    changed |= new_id != this->id;

    if (changed) {
        this->id = new_id;
    }

    return changed;
}

bool Sequence::activateMember(QString name, size_t time, size_t line)
{
    SEQMember* m = this->getMemberByName(name);
    // need to create new member
    if (!m) {
        this->members.push_back(SEQMember(name, line));
        m = &this->members.back();
    }

    // cannot activate already activated member
    if (m->getActivatedFlag())
        return false;

    // add new activation to stack
    m->setActivatedFlag(true);
    m->addActivation(time, line);
    return true;
}

bool Sequence::deactivateMember(QString name, size_t time)
{
    SEQMember* m = this->getMemberByName(name);

    // cannot deactivate non existing member
    if (!m) return false;

    // cannot deactivate already activated member
    if (!m->getActivatedFlag())
        return false;

    // update activation
    m->setActivatedFlag(false);
    m->setDeactivationTime(time);
    return true;
}

void Sequence::addAction(SEQAction action)
{
    SEQMember* sender = getMemberByName(action.getSender());
    if (sender == nullptr) {
        this->actions.push_back(action); // will get caught later
        return;
    }

    SEQMember* receiver = getMemberByName(action.getReceiver());
    if (receiver == nullptr) {
        this->actions.push_back(action); // will get caught later
        return;
    }

    if (!sender->getActivatedFlag()) {
        CodeService::formatLine(action.getLine(), HLevel::LEVEL_ERROR);
        VitaPrint("[ERROR]: Message sender was not active at the time message was sent.");
        action.setErrorLevel(3);
    }

    if (!receiver->getActivatedFlag()) {
        CodeService::formatLine(action.getLine(), HLevel::LEVEL_ERROR);
        VitaPrint("[ERROR]: Message receiver was not active at the time message was sent.");
        action.setErrorLevel(3);
    }

    this->actions.push_back(action); // OK
}

void Sequence::connectActions(std::vector<UMLClass> classes)
{
    for (size_t act = 0; act < actions.size(); act++) {
        if (actions[act].getType() == RuleID::R_NOP) continue;
        int i;
        i = getMemberIndexByName(actions[act].getSender());
        if (i != -1) {
            actions[act].setSenderIndex(i);
            if (!members[i].wasActiveAtTime(act)) {
                CodeService::formatLine(actions[act].getLine(), HLevel::LEVEL_ERROR);
                VitaPrint("[ERROR]: Message sender was not active at the time message was sent.");
                actions[act].setErrorLevel(2);
            }
            // OK
        }
        else {
            CodeService::formatLine(actions[act].getLine(), HLevel::LEVEL_ERROR);
            VitaPrint("[ERROR]: Message sender was not found in members.");
            actions[act].setErrorLevel(3);
            continue;
        }
        i = getMemberIndexByName(actions[act].getReceiver());
        if (i != -1) {
            actions[act].setReceiverIndex(i);
            if (!members[i].wasActiveAtTime(act)) {
                CodeService::formatLine(actions[act].getLine(), HLevel::LEVEL_ERROR);
                VitaPrint("[ERROR]: Message receiver was not active at the time message was sent.");
                actions[act].setErrorLevel(2);
            }
            // OK
        } else {
            CodeService::formatLine(actions[act].getLine(), HLevel::LEVEL_ERROR);
            VitaPrint("[ERROR]: Message receiver was not found in members.");
            actions[act].setErrorLevel(3);
            continue;
        }

    }
}

void Sequence::testActions(std::vector<UMLClass> classes)
{
    for (auto& action : actions) {
        if (action.getErrorLevel() != 0)
            continue;

        if (action.getType() == RuleID::R_ARROW_CALLBACK) {
            continue;
        }

        for (auto clas : classes) {
            if (clas.getClassName() == action.getReceiver()) {
                // match
                auto methods = clas.getMethods();
                auto inherited = clas.getInheritedMethods();
                bool isInside = false;

                QString type;
                for (UMLProperty m : methods) {
                    if (m.getName() == action.getMethod()) {
                        action.setMethodLine(m.pos);
                        type = m.getMod();
                        isInside = true;
                        break;
                    }
                }
                //not an own method, search in inherited
                if (!isInside) {
                    for (UMLProperty m : inherited) {
                        if (m.getName() == action.getMethod()) {
                            action.setMethodLine(m.pos);
                            type = m.getMod();
                            isInside = true;
                            break;
                        }
                    }
                }
                //not en inherited method, set error level
                if (!isInside) {
                    action.setErrorLevel(1);
                    CodeService::formatLine(action.getLine(), HLevel::LEVEL_ERROR);
                    VitaPrint("[ERROR]: Unknown method - not implemented.");
                }
                else {
                    //known, is it public?
                    if (type == QString("-")) {
                        if (action.getSenderIndex() != action.getReceiverIndex()) {
                            action.setErrorLevel(2);
                            CodeService::formatLine(action.getLine(), HLevel::LEVEL_ERROR);
                            VitaPrint("[ERROR]: Trying to access a private method from another entity.");
                        }
                    }
                    else if (type == QString("#"))
                    {
                        if (action.getSenderIndex() != action.getReceiverIndex()) {
                            //could be inherited
                            bool isInherited = false;
                            for (auto recRelation : clas.getRelations()) {
                                if (recRelation.getType() == RuleID::R_GEN && recRelation.getEntity() == action.getSender())
                                    isInherited = true;
                            }

                            if (!isInherited) {
                                action.setErrorLevel(2);
                                CodeService::formatLine(action.getLine(), HLevel::LEVEL_ERROR);
                                VitaPrint("[ERROR]: Trying to access a protected method from another entity.");
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}

SEQMember* Sequence::getMemberByName(QString name)
{
    for (auto& member : members) {
        if (member.getName() == name)
            return &member;
    }
    return nullptr;
}

void Sequence::testEntities(std::vector<UMLClass> classes)
{
    bool ok;
    for (auto& member : members) {
        ok = false;
        //is member in classes?
        for (size_t i = 0; i < classes.size(); i++) {
            if (classes[i].getClassName() == member.getName()) {
                ok = true;
                member.setClassID(i);
                member.setClassLine(classes[i].pos);
                member.setInterfaceFlag(classes[i].isInterface());
            }
        }

        member.setErrorFlag(!ok);

        if (!ok) {
            CodeService::formatLine(member.getLine(), HLevel::LEVEL_ERROR);
            VitaPrint("[ERROR]: Disconnected entity member.");
        }
    }
}

void Sequence::setEndIndexes(std::vector<UMLClass> classes)
{
    for (auto& act : actions) {
        act.setClassEndIndex(classes[members[act.getReceiverIndex()].getClassID()].pos_end);
    }
}

int Sequence::getMemberIndexByName(QString name)
{
    auto s = name.toStdString();
    int count = 0;
    for (auto member : members) {
        if (member.getName() == name)
            return count;
        count++;
    }
    return -1;
}

void Sequence::disableLeftovers(size_t time)
{
    for (auto& member : members) {
        deactivateMember(member.getName(), time);        
    }
}

void SEQMember::addActivation(size_t start, size_t startLine)
{
    this->activations.push_back(SEQActivation(start, startLine));
}

void SEQMember::setDeactivationTime(size_t time)
{
    this->activations.back().setEndIndex(time);
}


bool SEQMember::wasActiveAtTime(size_t time)
{
    for (SEQActivation act : activations) {
        if ((time >= act.startIndex()) && (time <= act.endIndex()))
            return true;
    }
    return false;
}
