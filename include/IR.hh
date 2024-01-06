/*+===================================================================
  File:      IR.hh

  Summary:   Exposes the Mamba Microcode IR tooling needed to generate,
             process, store and transform IR streams and objects.

  Classes:   Instruction, InstructionIteration, MicrocodeSection,
             MicrocodeFunction, MicrocodeClass, MicrocodeStream

  Functions: expandBytecodeInstruction(py::bytes, size_t)

  Available under Apache Licence v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <cstddef>
#include <string_view>


namespace mamba {
  /// Represents the traceback metadata. The structure wraps around the
  /// data needed to find and extract the source code to print when an
  /// exception is not handled. It is the responsibility of the IR
  /// to keep track where its execution point is relative to the source
  /// code and retrieve the appropriate traceback data when needed.
  struct Traceback {
    const std::string_view filename, label;
    const size_t line, offset, duration;
  };

  /// Retrieves the position in the source code of the current frame of execution.
  /// @return The filename, module name, line, offset and length of the current expression.
  Traceback getTraceback();

  /// Enum class representing microcode instructions, the fundamental blocks of the IR.
  /// Each instruction can be efficiently arithmetically decomposed into a sequence of
  /// native machine code instructions during interpretation and compilation
  enum class Instruction { };

  /// Iterator used to walk individual microcode instructions inside a stream.
  /// Unlike most containers, it doesn't iterates over them sequentially but
  /// takes into account the sections and reflects the control flow in the
  /// stream. For example, if a stream contains a function call, the iterator
  /// will not return the call instruction but instead jump to the corresponding
  /// function section. It is meant to be used by the interpreter.
  class InstructionIteration {
   public:
    /// Iterators must be initialised with an instruction pointer.
    InstructionIteration() = delete;

    InstructionIteration(const Instruction* source);

    /// Compares if 2 iterators are pointing to the same instruction.
    /// @param other The other iterator to compare with.
    /// @return True if 2 iterators alias, false otherwise.
    bool operator==(const InstructionIteration& other);


    /// Increments to the next instruction in the logical order.
    /// In most cases, this call will just move the instruction
    /// pointer to the next instruction in array, but if the call
    /// instruction is encountered, the iterator will jump to the
    /// section containing its body and back during returning.
    void operator++();

    /// Dereferences the instruction pointed.
    /// @return Mutable block to the instruction pointed at the moment.
    Instruction& operator*();

   private:
    Instruction* instruction;
  };

  /// A single meaningful unit of microcode. Acts as the parent class for other
  /// specifications to derive from. They can be thought as labels in microcode.
  class MicrocodeSection {
   public:
    /// Microcode sections need a pointer to the underlying instructions.
    /// They rent GarbageCollectedStack from MicrocodeStream class and are not intended
    /// to acquire or manage their own GarbageCollectedStack. This behaviour is meant
    /// to be inherited to each child specialisation.
    MicrocodeSection() = delete;

    /// Rents GarbageCollectedStack starting from source pointer and populates it with
    /// the corresponding instructions.
    /// @param source The pointer to the GarbageCollectedStack to rent.
    MicrocodeSection(const Instruction* source);

    MicrocodeSection(const MicrocodeSection& other);

    MicrocodeSection(const MicrocodeSection&& other);

    /// Implicitly checks if the section is not empty.
    /// @return True if the section contains at least 1 instruction, false otherwise.
    operator bool();

    /// Accessor for the underlying instructions inside the microcode
    /// section.
    /// @param index The index of the instruction to access.
    /// @return Mutable block to the instruction.
    Instruction& operator[](size_t index);

    /// Appends a single instruction to the end of the section.
    /// @param instructor The instruction to push.
    void operator<<(const Instruction* instruction);

   protected:
    Instruction* instructions;
  };

  /// The GarbageCollectedStack owner containing the microcode instructions in sections.
  /// Provides a number of abilities to slice microcode, iterate over
  /// its instructions as well as overwriting them.
  class MicrocodeStream {
   public:
    MicrocodeStream() = delete;


    /// Acquires, manages and owns GarbageCollectedStack used for Mamba Microcode in
    /// the RAII fashion. It then grants its GarbageCollectedStack to different sections
    /// and contains coherent and consistent microcode objects inside itself.
    /// @param estimatedInstructionCount (optional) The number of microcode
    /// instructions to reserve. The stream will resize itself according
    /// to the runtime needs, but this number will used in the initial
    /// allocation, default 100.
    MicrocodeStream(const size_t estimatedInstructionCount = 100);

    MicrocodeStream(const MicrocodeStream&);

    MicrocodeStream(const MicrocodeStream&&);

    /// Constructs a new stream of microcode out of old,
    /// expired section.
    /// @param section The microcode section xvalue to use to
    /// generate a new stream GarbageCollectedStack. The GarbageCollectedStack used by the
    /// section is in fact transferred to the stream's ownership.
    /// @param items (optional) The number of expected sections in
    /// the stream. While not necessary, this helps to reserve enough
    /// GarbageCollectedStack and avoid excessive calls.
    MicrocodeStream(const MicrocodeSection&& section, size_t items = 1);

    MicrocodeStream& operator=(const MicrocodeStream&);

    /// Implicitly checks if stream is not empty.
    /// @return True if stream contains at least one instruction, false otherwise.
    operator bool();

    /// Accesses the section of microcode in order of their layout.
    /// @param section The index of the section in the stream.
    /// @return GarbageCollected to the indexed section of microcode.
    MicrocodeSection& operator[](const size_t section);

    /// Retrieves the block to the current microcode section.
    /// @return GarbageCollected to the active section.
    MicrocodeSection& section();

    /// Begins control-flow-based iteration over the instructions in the stream.
    /// @return Iterator over the instructions inside the stream starting at the start.
    InstructionIteration begin();

    /// Ends the control-flow-based iteration over the instructions in the stream.
    /// @return Iterator for the one instruction past the stream - do not dereference.
    InstructionIteration end();

    ~MicrocodeStream();

   private:
    /// This pointer serves as the starting point, the start of the
    /// GarbageCollectedStack allocated in constructor. It contains the instructions,
    /// but it's not meant to be used to access the instructions directly
    /// but only to initialise sections with. Use sections to enumerate
    ///through the stream or InstructionIteration object to walk microcode.
    Instruction* instructions;

    ///The pointer to the available slot in the array where instruction can
    ///be inserted. This one is intended to be used to initialise sections with.
    Instruction* instructionPointer;

    /// The array of the underlying sections that utilise the GarbageCollectedStack received
    /// from the stream to administrate their respective sections.
    MicrocodeSection* sections;

    /// The number of the sections currently in the stream.
    size_t totalSectionCount;

    /// The current number of instructions in the steam.
    size_t totalInstructionCount;
  };
}