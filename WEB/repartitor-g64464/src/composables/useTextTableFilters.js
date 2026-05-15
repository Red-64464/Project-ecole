// Ce fichier gere les filtres et tris du tableau global des textes.
// Il evite de mettre toute cette logique directement dans TextsTable.vue.
// Comme ca, le composant reste plus facile a lire.
import { ref, computed } from "vue";

/**
 * Composable for filter/sort logic on the texts table (used in TextsTable.vue).
 * @param {import('vue').Ref<Array>} textsRef - reactive ref of the texts array
 */
export function useTextTableFilters(textsRef) {
  const filters = ref({
    cote: "",
    title: "",
    translator: "",
    editor: "",
    translated: "",
    edited: "",
  });
  const sortKey = ref("");
  const sortAsc = ref(true);

  function getLatestByAttributed(items = []) {
    return [...items].sort(
      (a, b) => new Date(b.attributed || 0) - new Date(a.attributed || 0),
    )[0];
  }
  function getTranslator(text) {
    return getLatestByAttributed(text.translation)?.translator || "";
  }
  function isTranslated(text) {
    return getLatestByAttributed(text.translation)?.finished != null;
  }
  function getEditor(text) {
    return getLatestByAttributed(text.edition)?.editor || "";
  }
  function isEdited(text) {
    return getLatestByAttributed(text.edition)?.finished != null;
  }
  function statusMatch(value, expected) {
    if (!expected) return true;
    return expected === "yes" ? value : !value;
  }

  const filteredTexts = computed(() => {
    let result = [...textsRef.value];

    if (filters.value.cote)
      result = result.filter((t) =>
        (t.cote || "").toLowerCase().includes(filters.value.cote.toLowerCase()),
      );
    if (filters.value.title)
      result = result.filter((t) =>
        (t.title || "")
          .toLowerCase()
          .includes(filters.value.title.toLowerCase()),
      );
    if (filters.value.translator)
      result = result.filter((t) =>
        getTranslator(t)
          .toLowerCase()
          .includes(filters.value.translator.toLowerCase()),
      );
    if (filters.value.editor)
      result = result.filter((t) =>
        getEditor(t).toLowerCase().includes(filters.value.editor.toLowerCase()),
      );

    result = result.filter(
      (t) =>
        statusMatch(isTranslated(t), filters.value.translated) &&
        statusMatch(isEdited(t), filters.value.edited),
    );

    if (sortKey.value) {
      result.sort((a, b) => {
        let valA = "",
          valB = "";
        switch (sortKey.value) {
          case "cote":
            valA = a.cote || "";
            valB = b.cote || "";
            break;
          case "title":
            valA = a.title || "";
            valB = b.title || "";
            break;
          case "precedent":
            valA = a.precedent || "";
            valB = b.precedent || "";
            break;
          case "received":
            valA = a.received || "";
            valB = b.received || "";
            break;
          case "deadline":
            valA = a.deadline || "";
            valB = b.deadline || "";
            break;
          case "translator":
            valA = getTranslator(a);
            valB = getTranslator(b);
            break;
          case "translated":
            valA = isTranslated(a) ? "1" : "0";
            valB = isTranslated(b) ? "1" : "0";
            break;
          case "editor":
            valA = getEditor(a);
            valB = getEditor(b);
            break;
          case "edited":
            valA = isEdited(a) ? "1" : "0";
            valB = isEdited(b) ? "1" : "0";
            break;
        }
        const cmp = String(valA).localeCompare(String(valB));
        return sortAsc.value ? cmp : -cmp;
      });
    }
    return result;
  });

  function toggleSort(key) {
    if (sortKey.value === key) {
      if (sortAsc.value) sortAsc.value = false;
      else {
        sortKey.value = "";
        sortAsc.value = true;
      }
    } else {
      sortKey.value = key;
      sortAsc.value = true;
    }
  }
  function clearFilter(key) {
    filters.value[key] = "";
  }
  function setStatusFilter(key, value) {
    filters.value[key] = filters.value[key] === value ? "" : value;
  }

  return {
    filters,
    filteredTexts,
    toggleSort,
    clearFilter,
    setStatusFilter,
    getTranslator,
    isTranslated,
    getEditor,
    isEdited,
  };
}
