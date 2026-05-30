local function augroup(name)
  return vim.api.nvim_create_augroup("my_" .. name, { clear = true })
end

---yank highlight----------
vim.api.nvim_create_autocmd("TextYankPost", {
  group = augroup("hightlight_light"),
  desc = "highlight when yank briefly",

  callback = function()
    vim.highlight.on_yank({ higroup = "IncSearch", timeout = 150 })
  end,
})

-- check whether the file has modified in other place --
vim.api.nvim_create_autocmd({ "FocusGained", "TermClose", "TermLeave" }, {
  group = augroup("checktime"),
  desc = "check whether the file has modified",

  callback = function()
    if vim.o.buftype ~= "nofile" then
      vim.cmd("checktime")
    end
  end,
})

--auto windows size-----
vim.api.nvim_create_autocmd("VimResized", {
  group = augroup("resize_splits"),
  desc = "auto resize the windows",

  callback = function()
    local current_tab = vim.fn.tabpagenr()
    vim.cmd("tabdo wincmd =")
    vim.cmd("tabnext " .. current_tab)
  end,
})

--automaticly create the directory when the parent directory isn't exit when saving file ----
vim.api.nvim_create_autocmd({ "BufWritePre" }, {
  group = augroup("auto_create_dir"),
  desc = "automaticly create dir when save file",

  callback = function(event)
    if event.match:match("^%w%w+:[\\/][\\/]") then
      return
    end
    local file = vim.uv.fs_realpath(event.match) or event.match
    local dir = vim.fn.fnamemodify(file, ":p:h")
    if vim.fn.isdirectory(dir) == 0 then
      vim.fn.mkdir(dir, "p")
    end
  end,
})

--built-in terminal setting
vim.api.nvim_create_autocmd("TermOpen", {
  group = augroup("terminal"),
  desc = "terminal setting",

  callback = function()
    vim.opt_local.number = false
    vim.opt_local.relativenumber = false
    vim.opt_local.signcolumn = "no"
  end,
})

--LspAttach------------
local map = vim.keymap

vim.api.nvim_create_autocmd("LspAttach", {
  group = augroup("lsp_attach"),
  desc = "LspAttach",

  callback = function(event)
    local client = vim.lsp.get_client_by_id(event.data.client_id)
    local bufnr = event.buf

    --only active when server support--
    if client:supports_method("textDocument/declaration", bufnr) then
      map.set("n", "gD", vim.lsp.buf.declaration, {
        buffer = bufnr,
        desc = "Lsp : jump to declaration",
      })
    end

    map.set("n", "gl", vim.diagnostic.open_float, {
      buffer = bufnr,
      desc = "Lsp : show the line diagnostic",
    })

    if client:supports_method("textDocument/inlayHint", bufnr) then
      map.set("n", "<leader>th", function()
        vim.lsp.inlay_hint.enable(not vim.lsp.inlay_hint.is_enabled({ bufnr = bufnr }), { bufnr = bufnr })
      end, {
        buffer = bufnr,
        desc = "Lsp : change Inlay Hints",
      })
    end
  end,
})

vim.api.nvim_create_autocmd("FileType", {
  group = augroup("treesitter"),
  desc = "automaticly active treesitter when file open",

  pattern = {
    "lua",
    "vim",
    "python",
    "javascript",
    "typescript",
    "rust",
    "go",
    "c",
    "cpp",
    "markdown",
    "html",
    "css",
    "json",
    "yaml",
    "toml",
    "bash",
  },

  callback = function(event)
    local ok, _ = pcall(vim.treesitter.language.add, event.match)
    if not ok then
      return
    end

    ok, _ = pcall(vim.treesitter.start, event.buf, event.match)
    if not ok then
      return
    end

    vim.wo.foldexpr = "v:lua.vim.treesitter.foldexpr()"
    vim.wo.foldmethod = "expr"

    vim.bo[event.buf].indentexpr = "v:lua.require('nvim-treesitter').indentexpr()"
  end,
})

-- 建立自動命令：確保每次加載自訂 base16 主題時，強制將檢查提示的背景變透明
vim.api.nvim_create_autocmd("ColorScheme", {
  pattern = "*",
  callback = function()
    -- 1. 需要清除背景色的語法檢查高亮組群
    local diagnostic_types = { "Error", "Warn", "Info", "Hint" }

    for _, type in ipairs(diagnostic_types) do
      -- 清除基礎組與行尾虛擬文字（Virtual Text）的背景色
      for _, suffix in ipairs({ "", "VirtualText" }) do
        local group_name = "Diagnostic" .. suffix .. type
        -- 獲取目前該組的屬性（用來提取當前的文字前景色 fg）
        local current = vim.api.nvim_get_hl(0, { name = group_name })

        -- 強制將背景 bg 設為 "NONE" (透明)，並保持原本的文字顏色 fg
        vim.api.nvim_set_hl(0, group_name, {
          fg = current.fg,
          bg = "NONE",
          ctermbg = "NONE",
        })
      end
    end

    -- 2. 額外修復：行號左側 SignColumn 的底色（如果你那邊有突兀的色塊）
    local signs = { "DiagnosticSignError", "DiagnosticSignWarn", "DiagnosticSignInfo", "DiagnosticSignHint" }
    for _, sign_group in ipairs(signs) do
      local current = vim.api.nvim_get_hl(0, { name = sign_group })
      vim.api.nvim_set_hl(0, sign_group, { fg = current.fg, bg = "NONE" })
    end
  end,
})

-- 立即手動觸發一次色彩更新，讓目前開啟的終端會話馬上生效
if vim.g.colors_name then
  vim.cmd([[colorscheme ]] .. vim.g.colors_name)
end
