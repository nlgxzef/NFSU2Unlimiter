#pragma once

// Taken from dbalatoni13/nfsmw

struct bNode
{
	bNode* Next;
	bNode* Prev;

	bNode* GetNext() {
		return Next;
	}

	bNode* GetPrev() {
		return Prev;
	}

	bNode* AddBefore(bNode* insert_point) {
		bNode* new_prev = insert_point->Prev;
		new_prev->Next = this;
		insert_point->Prev = this;
		this->Prev = new_prev;
		this->Next = insert_point;
		return this;
	}

	bNode* AddAfter(bNode* insert_point) {
		bNode* new_next = insert_point->Next;
		insert_point->Next = this;
		new_next->Prev = this;
		this->Prev = insert_point;
		this->Next = new_next;
		return this;
	}

	bNode* Remove() {
		bNode* next_node = this->Next;
		bNode* prev_node = this->Prev;
		prev_node->Next = next_node;
		next_node->Prev = prev_node;
		return this;
	}
};

template <typename T>
struct bTNode : bNode
{
	T* GetNext()
	{
		return (T*)bNode::GetNext();
	}

	T* GetPrev()
	{
		return (T*)bNode::GetPrev();
	}

	T* AddBefore(T* insert_point)
	{
		return (T*)bNode::AddBefore(insert_point);
	}

	T* AddAfter(T* insert_point)
	{
		return (T*)bNode::AddAfter(insert_point);
	}

	T* Remove()
	{
		return (T*)bNode::Remove();
	}
};

struct bList
{
	bNode HeadNode;

	void InitList()
	{
		this->HeadNode.Next = &this->HeadNode;
		this->HeadNode.Prev = &this->HeadNode;
	}

	int IsEmpty()
	{
		return static_cast<int>(this->HeadNode.GetNext() == &this->HeadNode);
	}

	bNode* EndOfList()
	{
		return &this->HeadNode;
	}

	bNode* GetHead()
	{
		return this->HeadNode.GetNext();
	}

	bNode* GetTail()
	{
		return this->HeadNode.GetPrev();
	}

	bNode* AddHead(bNode* node)
	{
		return node->AddAfter(&this->HeadNode);
	}

	bNode* AddTail(bNode* node)
	{
		return node->AddBefore(&this->HeadNode);
	}

	bNode* AddBefore(bNode* insert_point, bNode* node)
	{
		return node->AddBefore(insert_point);
	}

	bNode* AddAfter(bNode* insert_point, bNode* node)
	{
		return node->AddAfter(insert_point);
	}

	bNode* Remove(bNode* node)
	{
		return node->Remove();
	}

	bNode* RemoveHead()
	{
		return this->GetHead()->Remove();
	}
	bNode* RemoveTail()
	{
		return this->GetTail()->Remove();
	}

	int IsInList(bNode* node)
	{
		return this->TraversebList(node);
	}

	int CountElements()
	{
		return TraversebList(nullptr);
	}

	int TraversebList(bNode* match_node)
	{
		bNode* next = HeadNode.Next;
		int result = 0;

		while (next != &HeadNode)
		{
			if (next == match_node)
				return ++result;
			++result;
			next = next->Next;
		}

		if (match_node) return 0;
		return result;
	}
};

template <typename T>
struct bTList : bList
{
	T* GetHead()
	{
		return (T*)bList::GetHead();
	}

	T* GetTail()
	{
		return (T*)bList::GetTail();
	}
};