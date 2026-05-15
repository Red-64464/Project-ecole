// Ce fichier gere les filtres et tris des tableaux traducteur/editeur.
// Il evite de surcharger WorkerTextsTable.vue avec trop de logique.
// Il renvoie une liste deja filtree et triee.
import { ref, computed } from "vue";


/**
 * Composable for filter/sort logic on worker text tables (Edition / Traduction).
 * @param {import('vue').Ref<Array>} itemsRef - reactive ref of items
 * @param {'traduction'|'edition'} mode
 * @param {import('vue').Ref<Object>} statusesRef - translation statuses (edition only)
 */



export function useWorkerTableFilters(itemsRef, mode, statusesRef) {
  const filters = ref({
    cote: "",
    title: "",
    available: "",
    finished: "",
  });
  const sortKey = ref("");
  const sortAsc = ref(true);

  function statusMatch(value, expected) {
    if (!expected) return true;
    return expected === "yes" ? value : !value;
  }

  const filteredItems = computed(() => {
    let result = [...itemsRef.value];

    if (filters.value.cote)
      result = result.filter((item) =>
        (item.text?.cote || "")
          .toLowerCase()
          .includes(filters.value.cote.toLowerCase()),
      );
    if (filters.value.title)
      result = result.filter((item) =>
        (item.text?.title || "")
          .toLowerCase()
          .includes(filters.value.title.toLowerCase()),
      );

    if (mode === "edition") {
      result = result.filter(
        (item) =>
          statusMatch(
            Boolean(statusesRef?.value[item.text?.id]),
            filters.value.available,
          ) && statusMatch(Boolean(item.finished), filters.value.finished),
      );
    } else {
      result = result.filter((item) =>
        statusMatch(Boolean(item.finished), filters.value.finished),
      );
    }

    if (sortKey.value) {
      result.sort((a, b) => {
        let valA = "",
          valB = "";
        switch (sortKey.value) {
          case "cote":
            valA = a.text?.cote || "";
            valB = b.text?.cote || "";
            break;
          case "title":
            valA = a.text?.title || "";
            valB = b.text?.title || "";
            break;
          case "deadline":
            valA =
              (mode === "traduction" ? a.deadline : a.text?.deadline) || "";
            valB =
              (mode === "traduction" ? b.deadline : b.text?.deadline) || "";
            break;
          case "precedent":
            valA = a.text?.precedent || "";
            valB = b.text?.precedent || "";
            break;
          case "available":
            valA = statusesRef?.value[a.text?.id] ? "1" : "0";
            valB = statusesRef?.value[b.text?.id] ? "1" : "0";
            break;
          case "finished":
            valA = a.finished ? "1" : "0";
            valB = b.finished ? "1" : "0";
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

  return { filters, filteredItems, toggleSort, clearFilter, setStatusFilter };
}
