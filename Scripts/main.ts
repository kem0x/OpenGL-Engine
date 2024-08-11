// Give points to all friends around the target position
function onAbilityCast(caster: Unit, targetPos: Vector) {
    const units = findUnitsInRadius(targetPos, 500);

    const friends = units.filter(unit => caster.isFriend(unit));

    for (const friend of friends) {
        friend.givePoints(50);
    }

}