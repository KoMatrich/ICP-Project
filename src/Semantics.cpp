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
        this->members.push_back(SEQMember(name));
        m = &this->members.back();
    }

    // cannot activate already activated member
    if (m->getActivatedFlag())
        return false;

    // add new activation to stack
    m->setActivatedFlag(true);
    m->addActivation(time, line);
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
    this->actions.push_back(action);
}

SEQMember* Sequence::getMemberByName(QString name)
{
    for (auto& member : members) {
        if (member.getName() == name)
            return &member;
    }
    return nullptr;
}

void SEQMember::addActivation(size_t start, size_t startLine)
{
    this->activations.push_back(SEQActivation(start, startLine));
}

void SEQMember::setDeactivationTime(size_t time)
{
    this->activations.back().setEndIndex(time);
}
