#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Poison Touch has a 30% chance to poison when attacking with contact moves")
{
    PASSES_RANDOMLY(3, 10, RNG_POISON_TOUCH);
    GIVEN {
        ASSUME(GetMovePower(MOVE_SCRATCH) > 0);
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_POISON_TOUCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(player, ABILITY_POISON_TOUCH);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        MESSAGE("The opposing Wobbuffet was poisoned by Grimer's Poison Touch!");
        STATUS_ICON(opponent, poison: TRUE);
    }
}

SINGLE_BATTLE_TEST("Poison Touch only applies when using contact moves")
{
    u32 move;

    PARAMETRIZE { move = MOVE_SCRATCH; }
    PARAMETRIZE { move = MOVE_SWIFT; }
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        ASSUME(!MoveMakesContact(MOVE_SWIFT));
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_POISON_TOUCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        if (MoveMakesContact(move)) {
            ABILITY_POPUP(player, ABILITY_POISON_TOUCH);
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
            MESSAGE("The opposing Wobbuffet was poisoned by Grimer's Poison Touch!");
            STATUS_ICON(opponent, poison: TRUE);
        } else {
            NONE_OF {
                ABILITY_POPUP(player, ABILITY_POISON_TOUCH);
                ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
                MESSAGE("The opposing Wobbuffet was poisoned by Grimer's Poison Touch!");
                STATUS_ICON(opponent, poison: TRUE);
            }
        }
    }
}

SINGLE_BATTLE_TEST("Poison Touch applies between multi-hit move hits")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ARM_THRUST) == EFFECT_MULTI_HIT);
        ASSUME(MoveMakesContact(MOVE_ARM_THRUST));
        ASSUME(gItemsInfo[ITEM_PECHA_BERRY].holdEffect == HOLD_EFFECT_CURE_PSN);
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_POISON_TOUCH); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_PECHA_BERRY); };
    } WHEN {
        TURN { MOVE(player, MOVE_ARM_THRUST); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ARM_THRUST, player);
        ABILITY_POPUP(player, ABILITY_POISON_TOUCH);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        MESSAGE("The opposing Wobbuffet was poisoned by Grimer's Poison Touch!");
        STATUS_ICON(opponent, poison: TRUE);
        MESSAGE("The opposing Wobbuffet's Pecha Berry cured its poison!");
        STATUS_ICON(opponent, poison: FALSE);
        ABILITY_POPUP(player, ABILITY_POISON_TOUCH);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        MESSAGE("The opposing Wobbuffet was poisoned by Grimer's Poison Touch!");
        STATUS_ICON(opponent, poison: TRUE);
    }
}
