#ifndef TREE
#define TREE

#include <list>

template <typename Key, typename Type> class Tree;

template <typename Key, typename Type> class Tree {
public:
  struct Node {
  protected:
    Key varKey;
    Type varValue;
    Tree<Key, Type>::Node* pNodeParent;
    std::list<Tree<Key, Type>::Node*> listChildren;

  public:
    explicit Node(const Key& rvarInitializerA, const Type& rvarInitializerB);
    Node(const Tree<Key, Type>::Node& rNodeData) = delete;
    Node(Tree<Key, Type>::Node&& rNodeData) = delete;
    virtual ~Node(void);

    Tree<Key, Type>::Node& operator=(const Tree<Key, Type>::Node& rNodeData) = delete;
    Tree<Key, Type>::Node& operator=(Tree<Key, Type>::Node&& rNodeData) = delete;

    Key& GetKey(void);
    const Key& GetKey(void) const;
  
    Type& GetValue(void);
    const Type& GetValue(void) const;
  
    Tree<Key, Type>::Node*& GetParent(void);
    Tree<Key, Type>::Node* const& GetParent(void) const;

    std::list<typename Tree<Key, Type>::Node*>& GetChildren(void);
    const std::list<typename Tree<Key, Type>::Node*>& GetChildren(void) const;

    void SetKey(const Key& rvarNewKey);
    void SetValue(const Type& rvarNewValue);
  };

  static constexpr Tree<Key, Type>::Node* pNodeNull = nullptr;

protected:
  Tree<Key, Type>::Node* pNodeRoot;

public:
  explicit Tree(const Key& rvarInitializerA, const Type& rvarInitializerB);
  Tree(const Tree<Key, Type>& rTreeData);
  Tree(Tree<Key, Type>&& rTreeData);
  virtual ~Tree(void);

  Tree<Key, Type>& operator=(const Tree<Key, Type>& rTreeData);
  Tree<Key, Type>& operator=(Tree<Key, Type>&& rTreeData);

  Type& operator[](const Key& rvarTarget);
  const Type& operator[](const Key& rvarTarget) const;

  virtual Tree<Key, Type>::Node* const& GetRoot(void);
  virtual const Tree<Key, Type>::Node* const& GetRoot(void) const;

  virtual Tree<Key, Type>::Node* const& Insert(const Key& rvarTarget, const Key& rvarNewKey, const Type& rvarNewValue);
  virtual Tree<Key, Type>::Node* const& Erase(const Key& rvarTarget);

protected:
  virtual Tree<Key, Type>::Node* const& Search(const Key& rvarTarget);
  virtual const Tree<Key, Type>::Node* const& Search(const Key& rvarTarget) const;

  virtual void Copy(const Tree<Key, Type>& rTreeData);

public:
  virtual void Clear(void);

protected:
  virtual const Tree<Key, Type>::Node* const& SearchBase(const Tree<Key, Type>::Node* const& rpNodePointer, const Key& rvarTarget) const;

  virtual void CopyBase(const Tree<Key, Type>::Node* const& rpNodeSource, Tree<Key, Type>::Node* const& rpNodeDestination);

  virtual void ClearBase(Tree<Key, Type>::Node* const& rpNodePointer);
};

#endif