#include "Services/SSemantics.h"

void Semantics::printStack()
{
    for (size_t i = 0; i < this->stack.size(); i++) {
        QString out = QString("# %1: ").arg(i);

        if (this->stack[i].size() > 0) out.append(RuleIDtoString(this->stack[i][0].first->id));

        for (size_t j = 1; j < this->stack[i].size(); j++) {
            out.append(", ");
            out.append(RuleIDtoString(this->stack[i][j].first->id));
        }
        VitaPrint(out);
    }
}



void Semantics::testProperties()
{
    for (UMLClass& c : classes) {
        if (!c.has_changed) continue;

        //attributes
        auto att = c.getAttributes();

        for (size_t i = 0; i < att.size(); i++) {
            att[i].setDuplicateFlag(false);
        }

        //test own
        if (att.size() > 1) {
            for (size_t i = 0; i < att.size() - 1; i++) {
                for (size_t j = i + 1; j < att.size(); j++) {
                    if (att[i] == att[j]) {
                        if (!att[i].getDuplicateFlag()) {
                            VitaPrint("[ERROR] Duplicate attribute name: " + att[i].getName());
                        }


                        CodeService::formatLine(att[i].pos, HLevel::LEVEL_ERROR);
                        CodeService::formatLine(att[j].pos, HLevel::LEVEL_ERROR);
                        c.setErrorFlag(true);
                    }
                }
            }
        }

        //test inherited vs own
        auto inh_att = c.getInheritedAttributes();

        for (size_t i = 0; i < att.size(); i++) {
            att[i].setDuplicateFlag(false);
        }

        if ((att.size() > 0) && (inh_att.size() > 0)) {
            for (size_t i = 0; i < att.size(); i++) {
                for (size_t j = 0; j < inh_att.size(); j++) {
                    if (att[i] == inh_att[j]) {
                        if (!att[i].getDuplicateFlag())
                            VitaPrint("[ERROR] Duplicate inherited attribute name: " + att[i].getName());

                        CodeService::formatLine(att[i].pos, HLevel::LEVEL_ERROR);
                        CodeService::formatLine(att[j].pos, HLevel::LEVEL_ERROR);
                        c.setErrorFlag(true);
                    }
                }
            }
        }

        for (size_t i = 0; i < inh_att.size(); i++) {
            inh_att[i].setDuplicateFlag(false);
        }

        // test inherited vs inherited
        if (inh_att.size() > 1) {
            for (size_t i = 0; i < inh_att.size() - 1; i++) {
                for (size_t j = i + 1; j < inh_att.size(); j++) {
                    if (inh_att[i] == inh_att[j]) {
                        if (!inh_att[i].getDuplicateFlag())
                            VitaPrint("[ERROR] Inherited attribute in collision: " + inh_att[i].getName());

                        CodeService::formatLine(inh_att[i].pos, HLevel::LEVEL_ERROR);
                        CodeService::formatLine(inh_att[j].pos, HLevel::LEVEL_ERROR);
                        c.setErrorFlag(true);
                    }
                }
            }
        }

        //methods
        auto mth = c.getMethods();

        for (size_t i = 0; i < mth.size(); i++) {
            mth[i].setDuplicateFlag(false);
        }

        //test own
        if (mth.size() > 1) {
            for (size_t i = 0; i < mth.size() - 1; i++) {
                for (size_t j = i + 1; j < mth.size(); j++) {
                    if (mth[i] == mth[j]) {
                        if (!mth[i].getDuplicateFlag())
                            VitaPrint("[ERROR] Duplicate method name: " + mth[i].getName());

                        CodeService::formatLine(mth[i].pos, HLevel::LEVEL_ERROR);
                        CodeService::formatLine(mth[j].pos, HLevel::LEVEL_ERROR);
                        c.setErrorFlag(true);
                    }
                }
            }
        }

        //test inherited vs own
        auto inh_mth = c.getInheritedMethods();

        if ((mth.size() > 0) && (inh_mth.size() > 0)) {
            for (size_t i = 0; i < mth.size(); i++) {
                for (size_t j = 0; j < inh_mth.size(); j++) {
                    if (mth[i] == inh_mth[j]) {
                        if (!mth[i].getDuplicateFlag())
                            VitaPrint("[ERROR] Duplicate inherited method name: " + mth[i].getName());

                        CodeService::formatLine(mth[i].pos, HLevel::LEVEL_ERROR);
                        CodeService::formatLine(mth[j].pos, HLevel::LEVEL_ERROR);
                        c.setErrorFlag(true);
                    }
                }
            }
        }

        for (size_t i = 0; i < inh_mth.size(); i++) {
            inh_mth[i].setDuplicateFlag(false);
        }

        // test inherited vs inherited
        if (inh_mth.size() > 1) {
            for (size_t i = 0; i < inh_mth.size() - 1; i++) {
                for (size_t j = i + 1; j < inh_mth.size(); j++) {
                    if (inh_mth[i] == inh_mth[j]) {
                        if (!inh_mth[i].getDuplicateFlag())
                            VitaPrint("[ERROR] Inherited attribute in collision: " + inh_mth[i].getName());

                        CodeService::formatLine(inh_mth[i].pos, HLevel::LEVEL_ERROR);
                        CodeService::formatLine(inh_mth[j].pos, HLevel::LEVEL_ERROR);
                        c.setErrorFlag(true);
                    }
                }
            }
        }
    }
}

void Semantics::testRelations()
{
    for (size_t i = 0; i < classes.size(); i++) {
        auto& rel = classes[i].getRelations();
        for (size_t j = 0; j < rel.size(); j++) {
            UMLClass c;
            c.updateName(rel[j].getEntity());

            auto search = std::find(std::begin(this->classes), std::end(this->classes), c);

            if (search != std::end(this->classes)) {
                size_t index = search - this->classes.begin();
                classes[i].has_changed |= rel[j].updateRelationConnectors(index);
            }
            //not found entity
            else {
                classes[i].setErrorFlag(true);
                CodeService::formatLine(rel[j].pos, HLevel::LEVEL_WARN);
                VitaPrint("[WARNING]: Unknown entity relation: " + rel[j].toString());
                auto a = rel[j].toString().toStdString();

            }
        }
    }
}

void Semantics::addInheritedProperties()
{
    for (UMLClass& c : classes) {
        c.cleanAndSetUpdatedInherited();
        for (UMLRelation r : c.getRelations()) {
            if (r.getType() == RuleID::R_GEN) {
                c.updateInherited(true, classes[r.getID()].getMethods());
                c.updateInherited(false, classes[r.getID()].getAttributes());
            }
        }
    }
}

void Semantics::buildSTree(GlobalStack stack)
{
    if (stack.size() <= 1) return;

    HighlightService::setEnabled(false);
    //CodeService::clearBackground();

    if (stack.size() == 1) {
        if (stack[0].size() == 0) {
            VitaPrint("Write some code or open a document to get started.");
        }
    }
    this->stack = stack;
    //this->printStack();
    //VitaClear();

    size_t i = 0;
    size_t n = 0;
    this->skipTreeUntilLastIs({ RuleID::R_UML }, &i, 0);
    // find start of a class
    while (this->skipTreeUntilWhileTrue({ RuleID::R_CLASS, RuleID::R_INTERFACE }, &i, 1, RuleID::R_UML, 0)) {
        if (this->stack[i].size() < 4) {
            i++;
            continue;
        }

        QString c_name = this->stack[i][2].second;
        //auto aaa = c_name.toStdString();
        // update class
        if (n < this->classes.size()) {
            classes[n].updateName(c_name);
            classes[n].updateType(this->stack[i][1].first->id == RuleID::R_INTERFACE);
            classes[n].setErrorFlag(false);
            classes[n].removePosFlags();
            classes[n].pos = i;
        }
        // create class
        else {
            UMLClass c = UMLClass();
            c.updateName(c_name);
            c.updateType(this->stack[i][1].first->id == RuleID::R_INTERFACE);
            c.pos = i;
            this->addClass(c);
        }

        if (++i >= this->stack.size()) break;

        // we will iterate twice
        size_t saved_i = i;

        // always set to new index
        size_t a = 0;
        size_t m = 0;
        size_t r = 0;

        // get all attributes / methods
        while (this->skipTreeUntilWhileTrue({ RuleID::R_ACCESS }, &i, 4, RuleID::R_ENTITYBLOCK, 3)) {
            if (this->stack[i].size() == 7) {
                UMLProperty p = UMLProperty(this->stack[i][4].second, this->stack[i][5].second, this->stack[i][6].second);
                p.pos = i;

                if (this->stack[i][6].first->id == RuleID::R_METHOD)
                    this->classes[n].addProperty(p, true, m++);
                else
                    this->classes[n].addProperty(p, false, a++);
            } else {
                CodeService::formatLine(i, HLevel::LEVEL_WARN);
                VitaPrint("[WARNING]: Incomplete property definition (skipped)");
            }

            if (++i >= this->stack.size()) break;
        }
        this->classes[n].removeExceedingProperties(a, m);

        i = saved_i;

        // get all relations
        while (this->skipTreeUntilWhileTrue({ RuleID::R_IN }, &i, 4, RuleID::R_ENTITYBLOCK, 3)) {
            if (this->stack[i].size() == 8) {
                UMLRelation rel = UMLRelation(this->stack[i][7].second, this->stack[i][5].first->id);
                rel.pos = i;

                this->classes[n].addRelation(rel, r++);
            } else {
                CodeService::formatLine(i, HLevel::LEVEL_WARN);
                VitaPrint("[WARNING]: Incomplete relation definition (skipped)");
            }

            if (++i >= this->stack.size()) break;
        }

        this->classes[n].removeExceedingRelations(r);

        i = saved_i;
        this->classes[n].removePosLines();
        this->classes[n].removePos();
        // get all positions
        while (this->skipTreeUntilWhileTrue({ RuleID::R_XPOS, RuleID::R_YPOS }, &i, 4, RuleID::R_ENTITYBLOCK, 3)) {
            if (this->stack[i].size() == 6) {
                int pos = this->stack[i][5].second.toInt();
                // this can lead to error, so we need to test
                if (!this->classes[n].updatePosition(pos, this->stack[i][4].first->id == RuleID::R_XPOS, i))
                    void;
                //return;
            } else {
                CodeService::formatLine(i, HLevel::LEVEL_WARN);
                VitaPrint("[WARNING]: Incomplete position definition (skipped)");
            }

            if (++i >= this->stack.size()) break;
        }

        n++;
    }
    // delete excessive classes
    classes.resize(n);

    testDuplicates();
    testRelations();
    addInheritedProperties();
    testProperties();

    HighlightService::setEnabled(true);
}

void Semantics::addClass(UMLClass new_class)
{
    this->classes.push_back(new_class);
}

void Semantics::testDuplicates()
{
    if (classes.size() < 2) return; //no duplicates possible

    for (size_t i = 0; i < classes.size(); i++) {
        classes[i].setDuplicateFlag(false);
    }

    for (size_t i = 0; i < classes.size() - 1; i++) {
        for (size_t j = i + 1; j < classes.size(); j++) {
            if (classes[i] == classes[j]) {
                if (!classes[i].getDuplicateFlag())
                    VitaPrint("[ERROR] Duplicate entity name: " + classes[i].getClassName());

                CodeService::formatLine(classes[i].pos, HLevel::LEVEL_ERROR);
                classes[i].setErrorFlag(true);
                CodeService::formatLine(classes[j].pos, HLevel::LEVEL_ERROR);
                classes[j].setErrorFlag(true);
            }
        }
    }
}

bool Semantics::skipTreeUntilLastIs(std::vector<RuleID> rules, size_t* index, size_t pos)
{
    while (*index < this->stack.size()) {
        if (pos + 1 == this->stack[*index].size()) {
            if (std::find(std::begin(rules), std::end(rules), stack[*index][pos].first->id) != std::end(rules)) return true;
        }
        (*index)++;
    }
    return false;
}

bool Semantics::skipTreeUntilWhileTrue(std::vector<RuleID> rules, size_t* index, size_t pos, RuleID true_id, size_t true_pos)
{
    while (*index < this->stack.size()) {
        if (this->stack[*index].size() <= true_pos) return false;
        if (this->stack[*index][true_pos].first->id != true_id) return false;

        if (stack[*index].size() > pos) {
            if (std::find(std::begin(rules), std::end(rules), stack[*index][pos].first->id) != std::end(rules)) return true;
        }

        (*index)++;
    }
    return false;
}