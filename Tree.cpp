#include <string>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include "Definition.h"
#include "Tree.h"
#include "Skeleton Hierarchy.h"

template <typename Key, typename Type> Tree<Key, Type>::Node::Node(const Key& rvarInitializerA, const Type& rvarInitializerB) : varKey(rvarInitializerA), varValue(rvarInitializerB), pNodeParent(nullptr), listChildren(std::list<Tree<Key, Type>::Node*>())
{
}

template <typename Key, typename Type> Tree<Key, Type>::Node::~Node(void)
{
}

template <typename Key, typename Type> Key& Tree<Key, Type>::Node::GetKey(void)
{
  return varKey;
}

template <typename Key, typename Type> const Key& Tree<Key, Type>::Node::GetKey(void) const
{
  return varKey;
}

template <typename Key, typename Type> Type& Tree<Key, Type>::Node::GetValue(void)
{
  return varValue;
}

template <typename Key, typename Type> const Type& Tree<Key, Type>::Node::GetValue(void) const
{
  return varValue;
}

template <typename Key, typename Type> typename Tree<Key, Type>::Node*& Tree<Key, Type>::Node::GetParent(void)
{
  return pNodeParent;
}

template <typename Key, typename Type> typename Tree<Key, Type>::Node* const& Tree<Key, Type>::Node::GetParent(void) const
{
  return pNodeParent;
}

template <typename Key, typename Type> std::list<typename Tree<Key, Type>::Node*>& Tree<Key, Type>::Node::GetChildren(void)
{
  return listChildren;
}

template <typename Key, typename Type> const std::list<typename Tree<Key, Type>::Node*>& Tree<Key, Type>::Node::GetChildren(void) const
{
  return listChildren;
}

template <typename Key, typename Type> void Tree<Key, Type>::Node::SetKey(const Key& rvarNewKey)
{
  varKey = rvarNewKey;
}

template <typename Key, typename Type> void Tree<Key, Type>::Node::SetValue(const Type& rvarNewValue)
{
  varValue = rvarNewValue;
}

template <typename Key, typename Type> Tree<Key, Type>::Tree(const Key& rvarInitializerA, const Type& rvarInitializerB) : pNodeRoot(new Node(rvarInitializerA, rvarInitializerB))
{
}

template <typename Key, typename Type> Tree<Key, Type>::Tree(const Tree<Key, Type>& rTreeData) : pNodeRoot(new Node(rTreeData.GetRoot()->GetKey(), rTreeData.GetRoot()->GetValue()))
{
  Copy(rTreeData);
}

template <typename Key, typename Type> Tree<Key, Type>::Tree(Tree<Key, Type>&& rTreeData) : pNodeRoot(rTreeData.GetRoot())
{
  rTreeData.pNodeRoot = nullptr;
}

template <typename Key, typename Type> Tree<Key, Type>::~Tree(void)
{
  Clear();

  delete pNodeRoot;
}

template <typename Key, typename Type> Tree<Key, Type>& Tree<Key, Type>::operator=(const Tree<Key, Type>& rTreeData)
{
  if (&rTreeData == this)
    return *this;

  Clear();

  GetRoot()->GetKey() = rTreeData.GetRoot()->GetKey();
  GetRoot()->GetValue() = rTreeData.GetRoot()->GetValue();

  Copy(rTreeData);

  return *this;
}

template <typename Key, typename Type> Tree<Key, Type>& Tree<Key, Type>::operator=(Tree<Key, Type>&& rTreeData)
{
  if (&rTreeData == this)
    return *this;

  Clear();

  delete pNodeRoot;

  pNodeRoot = rTreeData.GetRoot();
  rTreeData.pNodeRoot = nullptr;

  return *this;
}

template <typename Key, typename Type> Type& Tree<Key, Type>::operator[](const Key& rvarTarget)
{
  return Search(rvarTarget)->GetValue();
}

template <typename Key, typename Type> const Type& Tree<Key, Type>::operator[](const Key& rvarTarget) const
{
  return Search(rvarTarget)->GetValue();
}

template <typename Key, typename Type> typename Tree<Key, Type>::Node* const& Tree<Key, Type>::GetRoot(void)
{
  return pNodeRoot;
}

template <typename Key, typename Type> const typename Tree<Key, Type>::Node* const& Tree<Key, Type>::GetRoot(void) const
{
  return pNodeRoot;
}

template <typename Key, typename Type> typename Tree<Key, Type>::Node* const& Tree<Key, Type>::Insert(const Key& rvarTarget, const Key& rvarNewKey, const Type& rvarNewValue)
{
  Tree<Key, Type>::Node* const& rpNodePointer = Search(rvarTarget);

  rpNodePointer->GetChildren().push_back(new Node(rvarNewKey, rvarNewValue));
  rpNodePointer->GetChildren().back()->GetParent() = rpNodePointer;

  return rpNodePointer->GetChildren().back();
}

template <typename Key, typename Type> typename Tree<Key, Type>::Node* const& Tree<Key, Type>::Erase(const Key& rvarTarget)
{
  Tree<Key, Type>::Node* const& rpNodePointer = Search(rvarTarget);

  if (rpNodePointer->GetParent() == nullptr)
    return rpNodePointer;
  else {
    typename std::list<Tree<Key, Type>::Node*>::iterator iterPosition = std::find(rpNodePointer->GetParent()->GetChildren().begin(), rpNodePointer->GetParent()->GetChildren().end(), rpNodePointer);

    if (rpNodePointer->GetChildren().size())
      rpNodePointer->GetParent()->GetChildren().splice(std::next(iterPosition), std::move(rpNodePointer->GetChildren()));

    iterPosition = rpNodePointer->GetParent()->GetChildren().erase(iterPosition);

    delete rpNodePointer;

    return *iterPosition;
  }
}

template <typename Key, typename Type> typename Tree<Key, Type>::Node* const& Tree<Key, Type>::Search(const Key& rvarTarget)
{
  return const_cast<Node* const&>(SearchBase(GetRoot(), rvarTarget));
}

template <typename Key, typename Type> const typename Tree<Key, Type>::Node* const& Tree<Key, Type>::Search(const Key& rvarTarget) const
{
  return SearchBase(GetRoot(), rvarTarget);
}

template <typename Key, typename Type> void Tree<Key, Type>::Copy(const Tree<Key, Type>& rTreeData)
{
  CopyBase(rTreeData.GetRoot(), GetRoot());
}

template <typename Key, typename Type> void Tree<Key, Type>::Clear(void)
{
  ClearBase(GetRoot());
}

template <typename Key, typename Type> const typename Tree<Key, Type>::Node* const& Tree<Key, Type>::SearchBase(const typename Tree<Key, Type>::Node* const& rpNodePointer, const Key& rvarTarget) const
{
  if (rpNodePointer->GetKey() == rvarTarget)
    return rpNodePointer;

  for (typename std::list<Tree<Key, Type>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++) {
    const Tree<Key, Type>::Node* const& rpNodeResult = SearchBase(*i, rvarTarget);

    if (rpNodeResult == nullptr)
      continue;
    else
      return rpNodeResult;
  }

  return reinterpret_cast<Tree<Key, Type>::Node* const&>(Tree<Key, Type>::pNodeNull);
}

template <typename Key, typename Type> void Tree<Key, Type>::CopyBase(const typename Tree<Key, Type>::Node* const& rpNodeSource, typename Tree<Key, Type>::Node* const& rpNodeDestination)
{
  rpNodeDestination->GetChildren().clear();

  for (typename std::list<Tree<Key, Type>::Node*>::const_iterator i = rpNodeSource->GetChildren().begin(); i != rpNodeSource->GetChildren().end(); i++) {
    rpNodeDestination->GetChildren().push_back(new Tree<Key, Type>::Node((*i)->GetKey(), (*i)->GetValue()));

    CopyBase(*i, rpNodeDestination->GetChildren().back());
  }
}

template <typename Key, typename Type> void Tree<Key, Type>::ClearBase(typename Tree<Key, Type>::Node* const& rpNodePointer)
{
  for (typename std::list<Tree<Key, Type>::Node*>::iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
    ClearBase(*i);

  rpNodePointer->GetChildren().clear();

  if (rpNodePointer == GetRoot()) {
    pNodeRoot->GetKey() = Key();
    pNodeRoot->GetValue() = Type();
  } else
    delete rpNodePointer;
}

template class Tree<std::string, PostureManager>;