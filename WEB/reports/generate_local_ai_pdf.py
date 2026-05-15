from datetime import datetime
from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_LEFT
from reportlab.lib.pagesizes import A4, landscape
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import cm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (
    PageBreak,
    Paragraph,
    SimpleDocTemplate,
    Spacer,
    Table,
    TableStyle,
)


ROOT = Path(__file__).resolve().parent
OUT = ROOT / "classement_modeles_ia_locaux_thinkpad_t14.pdf"

FONT = "SegoeUI"
FONT_BOLD = "SegoeUIBold"
pdfmetrics.registerFont(TTFont(FONT, r"C:\Windows\Fonts\segoeui.ttf"))
pdfmetrics.registerFont(TTFont(FONT_BOLD, r"C:\Windows\Fonts\segoeuib.ttf"))


styles = getSampleStyleSheet()
styles.add(
    ParagraphStyle(
        name="TitleFR",
        parent=styles["Title"],
        fontName=FONT_BOLD,
        fontSize=21,
        leading=25,
        alignment=TA_CENTER,
        textColor=colors.HexColor("#111827"),
        spaceAfter=10,
    )
)
styles.add(
    ParagraphStyle(
        name="H1FR",
        parent=styles["Heading1"],
        fontName=FONT_BOLD,
        fontSize=15,
        leading=18,
        textColor=colors.HexColor("#0f766e"),
        spaceBefore=10,
        spaceAfter=6,
    )
)
styles.add(
    ParagraphStyle(
        name="BodyFR",
        parent=styles["BodyText"],
        fontName=FONT,
        fontSize=9.8,
        leading=13,
        textColor=colors.HexColor("#1f2937"),
        alignment=TA_LEFT,
    )
)
styles.add(
    ParagraphStyle(
        name="SmallFR",
        parent=styles["BodyText"],
        fontName=FONT,
        fontSize=8.5,
        leading=11,
        textColor=colors.HexColor("#374151"),
    )
)
styles.add(
    ParagraphStyle(
        name="Cell",
        parent=styles["BodyText"],
        fontName=FONT,
        fontSize=7.2,
        leading=9,
        textColor=colors.HexColor("#111827"),
    )
)
styles.add(
    ParagraphStyle(
        name="CellBold",
        parent=styles["BodyText"],
        fontName=FONT_BOLD,
        fontSize=7.2,
        leading=9,
        textColor=colors.HexColor("#111827"),
    )
)


def p(text, style="BodyFR"):
    return Paragraph(text, styles[style])


def table(rows, widths, header=True):
    data = []
    for r_idx, row in enumerate(rows):
        converted = []
        for cell in row:
            converted.append(p(str(cell), "CellBold" if r_idx == 0 and header else "Cell"))
        data.append(converted)
    t = Table(data, colWidths=widths, repeatRows=1 if header else 0)
    t.setStyle(
        TableStyle(
            [
                ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#ccfbf1")),
                ("TEXTCOLOR", (0, 0), (-1, 0), colors.HexColor("#0f172a")),
                ("FONTNAME", (0, 0), (-1, 0), FONT_BOLD),
                ("GRID", (0, 0), (-1, -1), 0.35, colors.HexColor("#cbd5e1")),
                ("VALIGN", (0, 0), (-1, -1), "TOP"),
                ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.white, colors.HexColor("#f8fafc")]),
                ("LEFTPADDING", (0, 0), (-1, -1), 4),
                ("RIGHTPADDING", (0, 0), (-1, -1), 4),
                ("TOPPADDING", (0, 0), (-1, -1), 4),
                ("BOTTOMPADDING", (0, 0), (-1, -1), 4),
            ]
        )
    )
    return t


doc = SimpleDocTemplate(
    str(OUT),
    pagesize=landscape(A4),
    rightMargin=1.1 * cm,
    leftMargin=1.1 * cm,
    topMargin=1.0 * cm,
    bottomMargin=1.0 * cm,
)

story = []
story.append(p("Classement des meilleurs modeles IA locaux pour ton PC", "TitleFR"))
story.append(
    p(
        f"Rapport cree le {datetime.now().strftime('%d/%m/%Y')} pour un Lenovo ThinkPad T14 Gen 2a. "
        "But: choisir des modeles rapides, utiles pour coder, pour discuter, et pour analyser des images.",
        "BodyFR",
    )
)
story.append(Spacer(1, 0.15 * cm))

story.append(p("1. Resume simple de ton PC", "H1FR"))
spec_rows = [
    ["Element", "Valeur", "Ce que ca veut dire simplement"],
    ["PC", "Lenovo ThinkPad T14 Gen 2a - 20XLS27604", "Bon laptop pro, solide, mais pas une station IA avec grosse carte graphique."],
    ["CPU", "AMD Ryzen 5 PRO 5650U - 6 coeurs / 12 threads", "Correct pour lancer des modeles locaux, mais les gros modeles seront lents."],
    ["RAM", "40 Go DDR4 2667 MHz", "Tres bon point. Tu peux charger des modeles assez gros en RAM."],
    ["GPU", "AMD Radeon integre - environ 1 Go VRAM dedie", "Point faible pour l'IA locale. Pas de grosse VRAM, donc pas de turbo GPU."],
    ["Stockage", "SSD 512 Go, environ 203 Go libres", "Assez pour plusieurs modeles, mais evite de tout installer."],
    ["Ollama", "0.23.2 deja installe", "Parfait, tu peux installer les modeles directement."],
    ["Vitesse observee", "qwen3:8b autour de 5 tokens/s; gemma3:4b autour de 6 tokens/s", "Pour du fluide, vise surtout 2B a 8B."],
]
story.append(table(spec_rows, [4.1 * cm, 7.4 * cm, 14.8 * cm]))

story.append(p("2. Classement global recommande", "H1FR"))
global_rows = [
    ["Rang", "Modele", "Taille", "Image", "Thinking", "Vitesse sur ton PC", "Qualite", "Meilleur usage", "Commande"],
    ["1", "qwen2.5-coder:7b", "4.7 Go", "Non", "Non", "Rapide", "Tres bonne", "Coder tous les jours", "ollama pull qwen2.5-coder:7b"],
    ["2", "qwen3-vl:4b-thinking", "3.3 Go env.", "Oui", "Oui", "Tres rapide", "Tres bonne", "Images + raisonnement", "ollama pull qwen3-vl:4b-thinking"],
    ["3", "gemma3:4b", "3.3 Go", "Oui", "Non", "Tres rapide", "Bonne", "Chat rapide + images simples", "deja installe"],
    ["4", "qwen3:8b", "5.2 Go", "Non", "Oui selon mode", "Correct", "Tres bonne", "Assistant general intelligent", "deja installe"],
    ["5", "qwen3-vl:2b", "1.9 Go", "Oui", "Possible selon variante", "Excellent", "Bonne", "Image ultra fluide", "ollama pull qwen3-vl:2b"],
    ["6", "phi4-mini", "2.5 Go", "Non", "Non", "Excellent", "Bonne", "Assistant leger du quotidien", "ollama pull phi4-mini"],
    ["7", "llava-phi3:3.8b", "2.9 Go", "Oui", "Non", "Tres rapide", "Moyenne+", "Images legeres", "ollama pull llava-phi3"],
    ["8", "moondream:1.8b", "1.7 Go", "Oui", "Non", "Ultra rapide", "Simple", "Decrire une image vite", "ollama pull moondream"],
    ["9", "qwen2.5-coder:14b", "9 Go", "Non", "Non", "Moyen", "Tres bonne+", "Code plus dur", "ollama pull qwen2.5-coder:14b"],
    ["10", "deepseek-coder-v2:16b", "8.9 Go", "Non", "Non", "Moyen/lent", "Tres bonne", "Code avance", "ollama pull deepseek-coder-v2:16b"],
]
story.append(table(global_rows, [1.0 * cm, 4.0 * cm, 2.0 * cm, 1.6 * cm, 1.9 * cm, 2.9 * cm, 2.5 * cm, 4.0 * cm, 6.5 * cm]))

story.append(PageBreak())
story.append(p("3. Meilleurs modeles pour coder", "H1FR"))
code_rows = [
    ["Rang", "Modele", "Vitesse", "Force en code", "Pourquoi le choisir", "Mon verdict"],
    ["1", "qwen2.5-coder:7b", "Rapide", "Tres bonne", "Le meilleur equilibre sur ton PC: assez fort et pas trop lent.", "A installer en premier."],
    ["2", "qwen2.5-coder:14b", "Moyenne", "Tres bonne+", "Plus fort que le 7B, mais repondra moins vite.", "Pour les gros problemes."],
    ["3", "deepseek-coder-v2:16b", "Moyenne/lente", "Tres bonne", "Bon modele code MoE, mais moins fluide sur CPU.", "A tester apres le 7B."],
    ["4", "qwen3-coder:30b", "Lente", "Excellente", "Tres puissant mais trop gros pour une experience fluide sur ton laptop.", "Pas en modele principal."],
    ["5", "devstral:24b", "Lente", "Excellente", "Tres bon agent code, mais demande trop de puissance pour ton PC.", "A eviter si tu veux rapide."],
]
story.append(table(code_rows, [1.2 * cm, 4.5 * cm, 3.0 * cm, 3.0 * cm, 9.0 * cm, 5.2 * cm]))

story.append(p("4. Meilleurs modeles image + vision + thinking", "H1FR"))
vision_rows = [
    ["Rang", "Modele", "Image", "Thinking", "Vitesse", "Qualite image", "Utilisation ideale"],
    ["1", "qwen3-vl:4b-thinking", "Oui", "Oui", "Tres rapide", "Tres bonne", "Analyser captures d'ecran, UI, documents, erreurs, schemas."],
    ["2", "qwen3-vl:2b", "Oui", "Possible selon variante", "Excellent", "Bonne", "Quand tu veux une reponse tres rapide sur image."],
    ["3", "gemma3:4b", "Oui", "Non", "Tres rapide", "Bonne", "Questions simples sur images et chat quotidien."],
    ["4", "llava-phi3:3.8b", "Oui", "Non", "Tres rapide", "Moyenne+", "Petit modele vision classique."],
    ["5", "moondream:1.8b", "Oui", "Non", "Ultra rapide", "Simple", "Caption rapide: dire ce qu'il y a dans l'image."],
    ["6", "minicpm-v", "Oui", "Raisonnement image correct", "Moyenne", "Tres bonne", "Image plus poussee, mais moins fluide."],
    ["7", "qwen3-vl:8b", "Oui", "Oui selon variante", "Moyenne/lente", "Tres bonne+", "Plus fort, mais moins agreable sur ton PC."],
    ["8", "llama3.2-vision:11b", "Oui", "Non", "Lente", "Bonne", "Pas ideal pour ton PC, surtout sans GPU dedie."],
]
story.append(table(vision_rows, [1.2 * cm, 4.6 * cm, 1.6 * cm, 3.1 * cm, 3.0 * cm, 3.4 * cm, 9.6 * cm]))

story.append(p("5. A installer maintenant", "H1FR"))
install_rows = [
    ["Priorite", "Commande", "Pourquoi"],
    ["1", "ollama pull qwen2.5-coder:7b", "Ton meilleur modele local pour coder rapidement."],
    ["2", "ollama pull qwen3-vl:4b-thinking", "Ton meilleur modele image + thinking + fluide."],
    ["3", "ollama pull phi4-mini", "Assistant tres leger pour les petites questions."],
    ["4", "ollama pull moondream", "Vision ultra rapide si tu veux juste decrire une image."],
    ["5 optionnel", "ollama pull qwen2.5-coder:14b", "Plus fort en code, mais moins rapide."],
]
story.append(table(install_rows, [3.0 * cm, 8.0 * cm, 15.0 * cm]))

story.append(p("6. Modeles a eviter pour ton PC", "H1FR"))
avoid_rows = [
    ["Modele", "Pourquoi eviter"],
    ["qwen3-vl:30b / 32b", "Peut rentrer en RAM, mais trop lent pour ton objectif 'super fluide'."],
    ["qwen3-vl:235b", "Beaucoup trop gros."],
    ["llama3.2-vision:90b", "Trop lourd pour ton laptop."],
    ["llama3.1:70b", "Trop lent sans grosse carte graphique."],
    ["deepseek-coder-v2:236b", "Beaucoup trop gros."],
]
story.append(table(avoid_rows, [7.0 * cm, 19.0 * cm]))

story.append(Spacer(1, 0.2 * cm))
story.append(
    p(
        "Conclusion simple: installe qwen2.5-coder:7b pour coder, qwen3-vl:4b-thinking pour les images avec raisonnement, "
        "et garde gemma3:4b/qwen3:8b que tu as deja pour le quotidien. C'est le meilleur combo rapide pour ton ThinkPad.",
        "BodyFR",
    )
)
story.append(Spacer(1, 0.2 * cm))
story.append(
    p(
        "Sources principales: Ollama Library pour Qwen2.5-Coder, Qwen3-VL, Gemma 3, Llama 3.2 Vision, MiniCPM-V, Moondream; "
        "tests locaux effectues sur ta machine avec Ollama.",
        "SmallFR",
    )
)

doc.build(story)
print(OUT)
