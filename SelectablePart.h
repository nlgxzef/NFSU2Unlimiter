#pragma once

struct CarPart;

struct SelectablePart : bTNode<SelectablePart>
{
	CarPart* part;
	CAR_SLOT_ID slot;
};